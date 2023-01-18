#include "server.h"
#include "config.h"


#define AUTHENTICATION_DISABLED 1
#define IDENTIFICATION_DISABLED 1

/**
 * @brief Constructs the server and loads the config
 */
Server::Server() {
    // Load config here
    db = QSqlDatabase();
    config = Config();
    try {
        config.loadFromFile("/home/aljaz/Faks/2.letnik/0_zimski/RPO/vaje/Trenutni_GitHubRPO/RPOSystemMonitoringServer/cmake-build-debug/config.cfg");
    } catch (PropertyNotFoundException &e) {
        qDebug() << "Error loading config: " << e.what();
        throw e;
    } catch (FileException &e) {
        qDebug() << "Error loading config: " << e.what();
        throw e;
    }

    // Check if properties exist
    if (config.isEmpty()) {
        qDebug() << "Config is empty!";
        throw PropertyNotFoundException("Config is empty!");
    }
    if (!config.hasProperty("DatabaseUsername")) {
        qDebug() << "DatabaseUsername not found!";
        throw PropertyNotFoundException("DatabaseUsername not found!");
    }
    if (!config.hasProperty("DatabasePassword")) {
        qDebug() << "DatabasePassword not found!";
        throw PropertyNotFoundException("DatabasePassword not found!");
    }
    if (!config.hasProperty("DatabaseAddress")) {
        qDebug() << "DatabaseAddress not found!";
        throw PropertyNotFoundException("DatabaseAddress not found!");
    }
    if (!config.hasProperty("DatabasePort")) {
        qDebug() << "DatabasePort not found!";
        throw PropertyNotFoundException("DatabasePort not found!");
    }
    if (!config.hasProperty("DatabaseDB")) {
        qDebug() << "DatabaseDB not found!";
        throw PropertyNotFoundException("DatabaseDB not found!");
    }
    if (!config.hasProperty("Connector")) {
        qDebug() << "Connector not found!";
        throw PropertyNotFoundException("Connector not found!");
    }
    if (!config.hasProperty("ServerPort")) {
        qDebug() << "ServerPort not found!";
        throw PropertyNotFoundException("ServerPort not found!");
    }

    // DONE: Vzpostavi povezavo z bazo
    QString dbUsername = config.getProperty("DatabaseUsername");
    QString dbPassword = config.getProperty("DatabasePassword");
    QString dbAddress = config.getProperty("DatabaseAddress");
    QString dbPort = config.getProperty("DatabasePort");
    QString dbDB = config.getProperty("DatabaseDB");
    QString dbConnecter = config.getProperty("Connector");

    db = QSqlDatabase::addDatabase(dbConnecter);
    db.setPort(dbPort.toInt());
    db.setHostName(dbAddress);
    db.setDatabaseName(dbDB);
    db.setUserName(dbUsername);
    db.setPassword(dbPassword);
    bool ok = db.open();
    if(!ok) {
        qDebug() << "Povezava na BAZO ni uspela!\n";
    }

    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    // Load the config
    QString port;
    if (config.isEmpty()) {
        qDebug() << "Config is empty! Will use default values.";
        return;
    }
    port = config.getProperty("ServerPort");
    if (port.isEmpty()) {
        qDebug() << "Port is empty! Will use default value: 1234.";
        port = "1234";
    }

    if (!server->listen(QHostAddress::Any, port.toInt())) {
        qDebug() << "Server could not start!";
    } else {
        qDebug() << "Server started!";
    }
}


/**
 * @brief Deletes the server and disconnects all clients
 */
//https://www.youtube.com/watch?v=u5OdR46542M
Server::~Server() {
    server->close();
    server->deleteLater();

    for (auto &client: clients.keys()) {
        client->close();
    }
    clients.clear();
}
//https://www.bogotobogo.com/cplusplus/sockets_server_client.php
//https://www.youtube.com/watch?v=j9uAfTAZrdM
/**
 * @brief Accepts the new connection and saves it to a list
 */
void Server::onNewConnection() {
    // Add new client to the list
    auto *client = server->nextPendingConnection();

    // qDebug() << "User connected: " << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    // Create new session
    auto *session = new Session();
    clients.insert(client, session);
}

/**
 * @brief Reads the client message and processes it
 */

void Server::onReadReady() {
    auto client = reinterpret_cast<QTcpSocket *>(sender());

    QByteArray wholeData = client->readAll();

    parseMessage(client, wholeData);
}

bool Server::containsSocket(QTcpSocket *socket) {
    return clients.contains(socket);
}

/**
 *
 * @param clientSocket
 * če ni našel socket-a
 * @return null_ptr
 */
Session *Server::getSession(QTcpSocket *clientSocket) const {
    if (clients.contains(clientSocket)) {
        return clients.value(clientSocket);
    }
    return nullptr;
}

void Server::onDisconnected() {
    auto socket = reinterpret_cast<QTcpSocket *>(sender());
    if (clients.contains(socket))
        clients.remove(socket);
    socket->deleteLater();
}

void Server::parseMessage(QTcpSocket *client, const QByteArray &msg) {
    // TODO: IGNORE, Remove for production
    // qDebug() << "User " << client->peerAddress().toString() << " sent: " << msg;

    // 1: Authentication
    //    a) Accept connection
    //    b) Reject connection
    // 2: Identification
    //    a) Send new identification
    //    b) Send old identification
    // 3: New system
    //    a) Add new system
    //    b) Remove system and replace with new one
    // 4: Data
    //    a) Accept data and check if needed to be saved

    // TODO: IGNORE, Uncomment for production
//    if (client == nullptr || client->state() != QAbstractSocket::ConnectedState) {
//        qDebug() << "Client is null or not connected!";
//        return;
//    }

    auto messageTypes = Protocol::getMessageTypes(msg);
    auto messageJsons = Protocol::getMessageJsons(msg);
    if (messageTypes.size() != messageJsons.size()) {
        qDebug() << "Error: Message types and msg jsons are not the same size!";
        return;
    }
    if (messageTypes.empty()) {
        qDebug() << "Error: Message types and msg jsons are empty!";
        return;
    }

    for (int i = 0; i < messageTypes.size(); i++) {
        auto messageType = messageTypes.at(i);
        auto messageJson = messageJsons.at(i);

        /**
         * Checks for authentication
         */
        auto *session = clients.value(client);
        if (!session->isAuthenticated()) {
            bool confirm = false;
            if (messageType == MESSAGE::AUTHENTICATE) {
                auto username = Protocol::getUsername(messageJson);
                auto password = Protocol::getPassword(messageJson);
                auto res = authenticate(username, password);
                if (res != 0) {
                    confirm = true;
                    session->setUserId(res);
                }
            }
            auto json = Protocol::getConfirmationJson(confirm);
            auto message = Protocol::createMessage(MESSAGE::CONFIRM, json);
            // qDebug() << "Sending confirmation: " << message;
            client->write(message);
            if (!confirm) {
                client->close();
                return;
            }
            continue;
        }
        if (!session->isIdentified() && messageType == MESSAGE::IDENTIFY && session->isAuthenticated()) {
            bool confirm = false;
            auto clientId = Protocol::getClientId(messageJson);
            if (clientId == 0) {
                client->write(Protocol::createMessage(MESSAGE::CONFIRM, Protocol::getConfirmationJson(false)));
                return;
            }
            auto userId = session->getUserId();
            auto res = identify(userId, clientId);
            if (res != 0) {
                confirm = true;
                session->setClientId(clientId);
            }

            auto json = Protocol::getConfirmationJson(confirm);
            auto message = Protocol::createMessage(MESSAGE::CONFIRM, json);
            client->write(message);
            if (!confirm) {
                client->close();
                return;
            }
            continue;
        }
        if (session->isIdentified() && messageType == MESSAGE::SYSTEM && session->isAuthenticated() &&
            !session->isSystemAdded()) {
            auto system = Protocol::jsonToSystem(messageJson);
            if (system != nullptr) {
                client->write(Protocol::createMessage(MESSAGE::CONFIRM, Protocol::getConfirmationJson(false)));
                return;
            }
            auto clientId = session->getClientId();
            auto userId = session->getUserId();
            auto res = addSystem(userId, clientId, system);
            auto json = Protocol::getConfirmationJson(res);
            auto message = Protocol::createMessage(MESSAGE::CONFIRM, json);
            session->setSystemAdded(res);
            session->addSystem(system);
            client->write(message);
            continue;
        }
        if (session->isAuthenticated() && session->isIdentified() && session->isSystemAdded() &&
            messageType == MESSAGE::DATA) {
            auto data = Protocol::getMessageJsons(messageJson);
            auto clientId = session->getClientId();
            auto userId = session->getUserId();
//            auto res = addData(userId, clientId, data);
//            auto json = Protocol::getConfirmationJson(res);
//            auto message = Protocol::createMessage(MESSAGE::CONFIRM, json);
//            client->write(message);
            continue;
        }
    }
}

/*int Server::authenticate(const string &username, const string &password) {

    // TODO: IGNORE, Remove - for testing purposes only
    if (username == "test" && password == "test")
        return -2;
    // Ce najdes uporabnika v bazi vrni njegov IDENTIFY, ce ne vrni 0
    // -2 je samo za namene testiranja
}*/
//TODO izbriši ta komentar
/*
    query.value("SELECT username FROM user WHERE username = '" + userN + "'");
    while (query.next()) {
        QString country = query.value(0).toString();
        doSomething(country);
    }

    QSqlQuery q("select * from employees");
    QSqlRecord rec = q.record();

    qDebug() << "Number of columns: " << rec.count();

    int nameCol = rec.indexOf("name"); // index of the field "name"
    while (q.next())
        qDebug() << q.value(nameCol).toString(); // output all names


    */
int Server::authenticate1(const QByteArray &username) {
    db = QSqlDatabase();
    QSqlQuery query (db);

    qDebug() << "Tables: " << db.tables();
    int userID;
    std::string userN = username.toStdString();
    bool userInDB = query.exec(QString::fromStdString("SELECT username FROM user WHERE username = '" + userN + "'"));

    QString text = QString::fromStdString("SELECT username FROM user WHERE username = '" + userN + "'");

    qDebug() << "text: " << text << "\n";
    if(query.next()) { // both ==true
        userID = 1;
        qDebug() << "authenticate USPEŠNO: " << userID  << "," << userInDB << "\n";

        return userID;
    }
    else {
        userID = 1;
        qDebug() << "authenticate NI USPEŠNO: " << userID  << "," << userInDB << "\n";
        return 0;
    }
}

int Server::authenticate2(const QByteArray &password) {
    db = QSqlDatabase();
    QSqlQuery query (db);
    int userID;
    std::string pword = password.toStdString();
    bool userInDBp = query.exec(QString::fromStdString("SELECT password FROM user WHERE password = '" + pword + "'"));

    if(userInDBp) { // both ==true
        userID = 1;
        qDebug() << "authenticate USPEŠNO: " << userID  << "," << userInDBp << "\n";

        return userID;
    }
    else {
        userID = 1;
        qDebug() << "authenticate NI USPEŠNO: " << userID  << "," << userInDBp << "\n";
        return 0;
    }
}

int Server::authenticate(const QByteArray &username, const QByteArray &password) {
    // TODO: Poskrbi da se poveže najprej na bazo. DONE

    //if (!db.()) return 0;
    qDebug() << "Klic authenticate\n";
    // TODO: Preveri v PB uporabnika in geslo, ce se ne ujemata/ni v PB vrni 0, sicer IDENTIFY uporabnika (userID)
    db = QSqlDatabase();
    QSqlQuery query (db);
    int userID;
    std::string userN = username.toStdString();
    std::string pword = password.toStdString();

    //primerja geslo in username, prazen ni našel

    bool userInDB = query.exec(QString::fromStdString("SELECT username FROM user WHERE username = '" + userN + "'"));
    bool userInDBp = query.exec(QString::fromStdString("SELECT password FROM user WHERE password = '" + pword + "'"));

    //bool userInDBnp = query.exec(QString::fromStdString("SELECT username,password FROM user WHERE username = '" + userN + "' AND password = '" + pword + "'"));


    //QSqlQuery query("SELECT * FROM artist");
    if(userInDB && userInDBp) { // both ==true
    //if(userInDBnp) {
        //userID = query.record().indexOf(0);
        //QVariant userID_QV = query.value(QString::fromStdString("SELECT userID, username, password FROM user WHERE username = '" + userN + "' AND password = '" + pword + "'"));
        //userID = userID_QV.toInt();
        userID = 1;
        qDebug() << "authenticate USPEŠNO: " << userID << "\n";
        return userID;
    }
    else if(!userInDB) {
        userID = 1;
        qDebug() << "authenticate DELNO USPEŠNO: " << userID  << "," << userInDB << "," << userInDBp << "\n";
        //qDebug() << "authenticate USPEŠNO: " << userID << "\n";
        return userID;
    }
    else if(!userInDBp) {
        userID = 1;
        qDebug() << "authenticate SKORAJ USPEŠNO: " << userID  << "," << userInDB << "," << userInDBp << "\n";
        return userID;
    }

    else {
        userID = 1;
        qDebug() << "authenticate NI USPEŠNO: " << userID  << "," << userInDB << "," << userInDBp << "\n";

//        qDebug() << "authenticate NI USPEŠNO: " << userID  << "," << userInDBnp << "\n";
        return 0;
    }

}


int Server::identify(int userID, int clientID) {
    // TODO: IGNORE, Remove - for testing purposes only
    if (userID == -2 || clientID == -2)
        return -2;

    // TODO: Preveri, ce za tega uporbnika ze obstaja client ID, ce ne ga dodaj v PB in vrni nov ID, sicer vrni obstojeci ID
    //client
    //user
    db = QSqlDatabase();
    QSqlQuery query (db);
    std::string uID = std::to_string(userID);
    std::string cID = std::to_string(clientID);
    bool clientIDInDB = query.exec(QString::fromStdString("SELECT userID, clientID FROM credential WHERE userID = '" + uID + "' AND clientID = '" + cID + "'"));
    //zadnji clientID, ki je bil vnešen
    QVariant clientID_QV = query.lastInsertId();
    clientID = clientID_QV.toInt();


/*
    //if(!clientIDInDB) {
        //dodajanje clientID če ga ni v bazi
    if (!query.next()) {
        clientID = clientID+1; //naslednji
        query.exec(QString::fromStdString("INSERT INTO credential VALUES (
                                              clientID +  userID)"));
        db.commit();
        return clientID;
    }

    else {
        QVariant clientID_QV = query.value(QString::fromStdString("SELECT userID, clientID FROM credential WHERE clientID = '" + cID + "'"));
        clientID = clientID_QV.toInt();
        return clientID;
    }
    */
}

bool Server::addSystem(int userID, int clientID, System *system) {
    // TODO: Dodaj sistem v PB in vrni true, ce je uspesno dodan (oz posodobljen), sicer vrni false

    return true;
}



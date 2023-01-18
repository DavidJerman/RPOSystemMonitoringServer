//HELP klikneš gor in pritisneš F1

#include "server.h"
#include "config.h"

#define AUTHENTICATION_DISABLED 1
#define IDENTIFICATION_DISABLED 1

/**
 * @brief Constructs the server and loads the config
 */
Server::Server() {
    // Load config here
    config = Config();
    try {
        config.loadFromFile("/home/pinkynyte/CLionProjects/RPOSystemMonitoringServer/cmake-build-debug/config.cfg");
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

    QString dbUsername = config.getProperty("DatabaseUsername");
    QString dbPassword = config.getProperty("DatabasePassword");
    QString dbAddress = config.getProperty("DatabaseAddress");
    QString dbPort = config.getProperty("DatabasePort");
    QString dbDB = config.getProperty("DatabaseDB");
    QString dbConnector = config.getProperty("Connector");

    db = QSqlDatabase::addDatabase(dbConnector);
    db.setDatabaseName(dbDB);
    db.setHostName(dbAddress);
    db.setPort(dbPort.toInt());
    db.setUserName(dbUsername);
    db.setPassword(dbPassword);
    bool ok = db.open();
    if (!ok) {
        qDebug() << "Error connecting to database!";
        throw DatabaseException("Error connecting to database!");
    } else {
        qDebug() << "Connected to database: " << db.databaseName();
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
Server::~Server() {
    server->close();
    server->deleteLater();

    for (auto &client: clients.keys()) {
        client->close();
    }
    clients.clear();
    db.close();
}


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
 * @brief Reads the data from the client and sends it to the parser
 * @param socket The socket of the client
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
 * @brief Returns the session for the given socket
 * @param clientSocket The socket to get the session for
 * @return The session for the given socket
 */
Session *Server::getSession(QTcpSocket *clientSocket) const {
    if (clients.contains(clientSocket)) {
        return clients.value(clientSocket);
    }
    return nullptr;
}


/**
 * @brief Removes the client from the list
 */
void Server::onDisconnected() {
    auto socket = reinterpret_cast<QTcpSocket *>(sender());
    if (clients.contains(socket))
        clients.remove(socket);
    socket->deleteLater();
}


/**
 * @brief Parses the message and processes it
 * @param client
 * @param message
 */
void Server::parseMessage(QTcpSocket *client, const QByteArray &msg) {
    // TODO: Remove for production
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

    // TODO: Uncomment for production
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
            // TODO: More checks
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
            qDebug() << "<Server> Data";
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


/**
 * @brief Authenticates the user
 * @param username
 * @param password
 * @return 0 if not authenticated, user id if authenticated
 */
int Server::authenticate(const QByteArray &username, const QByteArray &password) {
    // TODO: Session
    // TODO: Preveri v PB uporabnika in geslo, ce se ne ujemata/ni v PB vrni 0, sicer IDENTIFY uporabnika (userID)
    QSqlQuery query;
    // TODO: Save passwords as hashes, safer
    if (!query.exec("SELECT * FROM user WHERE username = '" + username + "' AND password = '" + password + "'")) {
        qDebug() << "Error: " << query.lastError().text();
        return 0;
    }
    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}


/**
 * @brief Identifies user
 * @param userId User ID
 * @param clientId Client ID
 * @return 0 if failed, 1 if success
 */
int Server::identify(int userID, int clientID) {
    // TODO: Session
    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM client WHERE fk_user = " + QString::number(userID) + " AND id = " +
                    QString::number(clientID))) {
        qDebug() << "Error: " << query.lastError().text();
        qDebug() << db.databaseName();
        return 0;
    }

    if (query.next()) {
        return clientID;
    } else {
        // Insert a new client
        if (!query.exec(
                "INSERT INTO client (fk_user, name, operating_system, system_type) VALUES (" + QString::number(userID) +
                ", 'test', 'test', 'test')")) {
            qDebug() << "Error: " << query.lastError().text();
            qDebug() << db.databaseName();
            return 0;
        }
        db.commit();
    }

    // TODO: Remove - for testing purposes only
    return 0;
}


/**
 * @brief Adds a new system to the database
 * @param userID User ID
 * @param clientID Client ID
 * @param system System to be added
 * @return 0 if error, 1 if success
 */
bool Server::addSystem(int userID, int clientID, System *system) {
    // TODO: Case when system clientID is different from clientID
    for (const auto &component: system->getComponents()) {
        // If cpu
        if (dynamic_cast<Cpu *>(component)) {
            auto cpu = dynamic_cast<Cpu *>(component);
            QSqlQuery query(db);
            if (cpu->getId() == 0) {
                // If the same cpu is already in the database
                if (query.exec("SELECT * FROM cpu WHERE name = '" + cpu->getName()
                               + "' AND max_frequency = '" + QString::number(cpu->getMaxFrequency())
                               + "' AND cores = '" + QString::number(cpu->getCores())
                               + "' AND fk_client = " + QString::number(clientID))) {
                    if (query.next()) {
                        cpu->setId(query.value(0).toInt());
                        continue;
                    }
                } else {
                    qDebug() << "Error: " << query.lastError().text();
                    return false;
                }
                if (!query.exec(
                        "INSERT INTO cpu (name, max_frequency, cores, fk_client) VALUES ('" + cpu->getName() + "', " +
                        QString::number(cpu->getMaxFrequency()) + ", " + QString::number(cpu->getCores()) + ", " +
                        QString::number(clientID) + ")")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            } else {
                if (!query.exec("SELECT * FROM cpu WHERE fk_client = " + QString::number(clientID) + " AND id = '" +
                                QString::number(cpu->getId()) + "'")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            }
            db.commit();
        } else if (dynamic_cast<Disk *>(component)) {
            auto disk = dynamic_cast<Disk *>(component);
            QSqlQuery query(db);
            if (disk->getId() == 0) {
                // If the same disk is already in the database
                if (query.exec("SELECT * FROM disk WHERE name = '" + disk->getName()
                               + "' AND capacity = '" + QString::number(disk->getCapacity())
                               + "' AND type = '" + disk->getType()
                               + "' AND fk_client = " + QString::number(clientID))) {
                    if (query.next()) {
                        disk->setId(query.value(0).toInt());
                        continue;
                    }
                } else {
                    qDebug() << "Error: " << query.lastError().text();
                    return false;
                }
                if (!query.exec(
                        "INSERT INTO disk (name, capacity, type, fk_client) VALUES ('" + disk->getName() + "', " +
                        QString::number(disk->getCapacity()) + ", '" + disk->getType() + "', " +
                        QString::number(clientID) + ")")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            } else {
                if (!query.exec("SELECT * FROM disk WHERE fk_client = " + QString::number(clientID) + " AND id = '" +
                                QString::number(disk->getId()) + "'")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            }
            db.commit();
        } else if (dynamic_cast<Gpu *>(component)) {
            auto gpu = dynamic_cast<Gpu *>(component);
            QSqlQuery query(db);
            if (gpu->getId() == 0) {
                // If the same gpu is already in the database
                if (query.exec("SELECT * FROM gpu WHERE name = '" + gpu->getName()
                               + "' AND max_frequency = '" + QString::number(gpu->getMaxFrequency())
                               + "' AND capacity = '" + QString::number(gpu->getVRam())
                               + "' AND fk_client = " + QString::number(clientID))) {
                    if (query.next()) {
                        gpu->setId(query.value(0).toInt());
                        continue;
                    }
                } else {
                    qDebug() << "Error: " << query.lastError().text();
                    return false;
                }
                if (!query.exec(
                        "INSERT INTO gpu (name, max_frequency, capacity, fk_client) VALUES ('" + gpu->getName() +
                        "', " + QString::number(gpu->getMaxFrequency()) + ", " + QString::number(gpu->getVRam()) +
                        ", " + QString::number(clientID) + ")")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            } else {
                if (!query.exec("SELECT * FROM gpu WHERE fk_client = " + QString::number(clientID) + " AND id = '" +
                                QString::number(gpu->getId()) + "'")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            }
        } else if (dynamic_cast<Network *>(component)) {
            auto network = dynamic_cast<Network *>(component);
            QSqlQuery query(db);
            if (network->getId() == 0) {
                // If the same network is already in the database
                if (query.exec("SELECT * FROM network WHERE name = '" + network->getName()
                               + "' AND type = '" + network->getType()
                               + "' AND fk_client = " + QString::number(clientID))) {
                    if (query.next()) {
                        network->setId(query.value(0).toInt());
                        continue;
                    }
                } else {
                    qDebug() << "Error: " << query.lastError().text();
                    return false;
                }
                if (!query.exec("INSERT INTO network (name, type, fk_client) VALUES ('" + network->getName() + "', '" +
                                network->getType() + "', " + QString::number(clientID) + ")")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            } else {
                if (!query.exec("SELECT * FROM network WHERE fk_client = " + QString::number(clientID) + " AND id = '" +
                                QString::number(network->getId()) + "'")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            }
        } else if (dynamic_cast<Ram *>(component)) {
            auto ram = dynamic_cast<Ram *>(component);
            QSqlQuery query(db);
            if (ram->getId() == 0) {
                // If the same ram is already in the database
                if (query.exec("SELECT * FROM ram WHERE name = '" + ram->getName()
                               + "' AND capacity = '" + QString::number(ram->getCapacity())
                               + "' AND type = '" + ram->getType()
                               + "' AND frequency = '" + QString::number(ram->getFrequency())
                               + "' AND fk_client = " + QString::number(clientID))) {
                    if (query.next()) {
                        ram->setId(query.value(0).toInt());
                        continue;
                    }
                } else {
                    qDebug() << "Error: " << query.lastError().text();
                    return false;
                }
                if (!query.exec(
                        "INSERT INTO ram (name, capacity, type, frequency, fk_client) VALUES ('" + ram->getName() +
                        "', " + QString::number(ram->getCapacity()) + ", '" + ram->getType() + "', " +
                        QString::number(ram->getFrequency()) + ", " + QString::number(clientID) + ")")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            } else {
                if (!query.exec("SELECT * FROM ram WHERE fk_client = " + QString::number(clientID) + " AND id = '" +
                                QString::number(ram->getId()) + "'")) {
                    qDebug() << "Error: " << query.lastError().text();
                    qDebug() << db.databaseName();
                    return false;
                }
            }
        }
    }
    return true;
}

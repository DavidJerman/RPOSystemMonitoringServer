#include "server.h"
#include "config.h"

/**
 * @brief Constructs the server and loads the config
 */
Server::Server() {
    // Load config here

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

int Server::authenticate(QTcpSocket *client, const QByteArray& username, const QByteArray& password) {
    // Ce najdes uporabnika v bazi vrni njegov ID, ce ne vrni 0
    return 0;
}


int Server::identify(int userID, int clientID) {
    // TODO: Preveri, ce za tega uporbnika ze obstaja client ID, ce ne ga dodaj v PB in vrni nov ID, sicer vrni obstojeci ID

    return 1;
}

bool Server::addSystem(int userID, int clientID, System *system) {
    // TODO: Dodaj sistem v PB in vrni true, ce je uspesno dodan (oz posodobljen), sicer vrni false

    return true;
}

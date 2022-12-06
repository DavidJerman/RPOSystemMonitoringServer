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
    Config config;
    QString port;
    try {
        config.loadFromFile("config.cfg");
        if (config.isEmpty()) {
            qDebug() << "Config is empty! Will use default values.";
            return;
        }
        port = config.getProperty("port");
        if (port.isEmpty()) {
            qDebug() << "Port is empty! Will use default value: 1234.";
            port = "1234";
        }
    } catch (const std::exception& e) {
        qDebug() << "Error loading config: " << e.what();
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

    qDebug() << "User connected: " << client->peerAddress().toString();

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

    qDebug() << "User " << client->peerAddress().toString() << " sent: " << wholeData;

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

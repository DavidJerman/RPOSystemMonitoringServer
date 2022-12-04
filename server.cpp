#include "server.h"

/**
 * @brief Constructs the server and loads the config
 */
Server::Server()
{
    // Load config here

    server = new QTcpServer(this);

    connect (server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    // TODO: Load config from file
    if(!server->listen(QHostAddress::Any,1234)) {
        qDebug() << "Server se ni zmogel zagnat.";
    }
    else {
        qDebug() << "Server se je zagnal.";
    }

}


/**
 * @brief Deletes the server and disconnects all clients
 */
//https://www.youtube.com/watch?v=u5OdR46542M
Server::~Server()
{
    server->close();
    server->deleteLater();

    // TODO: Disconnect all clients
    for (auto client : clients) {
        client->close();
        //onDisconnected() ??
    }
}
//https://www.bogotobogo.com/cplusplus/sockets_server_client.php
//https://www.youtube.com/watch?v=j9uAfTAZrdM
/**
 * @brief Accepts the new connection and saves it to a list
 */
void Server::onNewConnection()
{
    // Add new client to the list
    auto *client = server->nextPendingConnection();

    connect(client, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    // Connect readyRead

    clients.append(client);
}

/**
 * @brief Reads the client message and processes it
 */

void Server::onReadReady()
{
    auto client = reinterpret_cast<QTcpSocket*>(sender());
        QByteArray wholeData = client->readAll();
}

//https://en.cppreference.com/w/cpp/language/reinterpret_cast
//https://doc.qt.io/archives/qt-4.8/signalsandslots.html#advanced-signals-and-slots-usage
//https://socket.io/docs/v3/server-socket-instance/
//https://socket.io/docs/v3/rooms/
void Server::onDisconnected()
{   //QTcpSocket socket
    auto client = reinterpret_cast<QTcpSocket*>(sender());
        //log(getID(client) + " has disconnected!");
        if (clients.contains(client))
            clients.remove(clients.indexOf(client));
        client->deleteLater();
        client = nullptr;
    exit(0);
}

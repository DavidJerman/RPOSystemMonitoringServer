//HELP klikneš gor in pritisneš F1

#include "server.h"
//#include "config.h"


/**
 * @brief Constructs the server and loads the config
 */
Server::Server(QObject *parent): QObject(parent)
{
    // Load config here

    // TODO: Create QTcpServer properly
    server = new QTcpServer(this);

    connect (server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));


    if(!server->listen(QHostAddress::Any,1234)) {
        qDebug() << "Server se ni zmogel zagnat.";
    }
    else {
        qDebug() << "Server se je zagnal.";
    }

}


/**
 * @brief Deletes the server and disconnectes all clients
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
    // private: QList<QTcpSocket*> clients;
    // Add new client to the list
    auto *client = server->nextPendingConnection();

    connect(client, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    // Connect readyRead

    clients.append(client);
    // Remove thread from the list at the end... else CRASH
    //clients.insert(client, "Unknown");

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



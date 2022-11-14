#include "server.h"

Server::Server()
{
    // TODO: Create QTcpServer
    server = new QTcpServer();
}


Server::~Server()
{
    server->close();
    server->deleteLater();

    // TODO: Disconnect all clients
    for (auto client : clients) {
        // TODO
    }
}


// New connection slot
void Server::onNewConnection()
{
    // Add new client to the list
    auto client = server->nextPendingConnection();
    clients.append(client);
}


// Ready read slot
void Server::onReadReady()
{
    // Accept
}

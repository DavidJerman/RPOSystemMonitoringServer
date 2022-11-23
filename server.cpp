#include "server.h"

/**
 * @brief Constructs the server and loads the config
 */
Server::Server()
    : server(new QTcpServer()),
      config(Config())
{
    // Load config
    config.loadFromFile("./config.cfg");

    // TODO: Check for needed config values
}


/**
 * @brief Deletes the server and disconnectes all clients
 */
Server::~Server()
{
    server->close();
    server->deleteLater();

    // TODO: Disconnect all clients
    for (auto client : clients) {
        // TODO
    }
}


/**
 * @brief Accepts the new connection and saves it to a list
 */
void Server::onNewConnection()
{
    // Add new client to the list
    auto client = server->nextPendingConnection();
    clients.append(client);
}


/**
 * @brief Reads the client message and processes it
 */
void Server::onReadReady()
{
    // Accept
}

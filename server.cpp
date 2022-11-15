#include "server.h"
#include "config.h"


/**
 * @brief Constructs the server and loads the config
 */
Server::Server()
{
    // Load config here

    // TODO: Create QTcpServer properly
    server = new QTcpServer();

    // Temp
    // Just for testing
    Config config;

    try {
        config.getValue("Test");

    }
    catch (const PropertyNotFoundException& e)
    {
        qDebug() << e.message();
    }

    try {
        config.loadFromFile("test.txt");
    }
    catch (const FileException& e)
    {
        qDebug() << e.message();
    }

    auto properties = config.getProperties();
    for (auto &property : properties)
    {
        qDebug() << property << config.getValue(property);
    }
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

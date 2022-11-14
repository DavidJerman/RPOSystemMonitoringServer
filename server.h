#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server
{
public:
    Server();

public slots:
    void onNewConnection();

    void onReadReady();

private:
    QTcpServer* server;
};

#endif // SERVER_H

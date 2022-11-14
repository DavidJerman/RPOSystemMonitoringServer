#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

#include <memory>

class Server
{
public:
    Server();

    ~Server();

public slots:
    // Slots that we connect to signals
    void onNewConnection();

    void onReadReady();

private:
    QTcpServer* server;

    QList<QTcpSocket*> clients;
};

#endif // SERVER_H

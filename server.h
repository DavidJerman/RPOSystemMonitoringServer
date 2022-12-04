#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

#include <memory>

class Server : public QObject
{
    Q_OBJECT
public:
    //Server();
    explicit Server(QObject *parent = 0);

    //virtual ~Server();
    ~Server();

public slots:
    // Slots that we connect to signals
    void onNewConnection();

    void onReadReady();
    void onDisconnected();


private:
    QTcpServer* server;

    QList<QTcpSocket*> clients;
};

#endif // SERVER_H


#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSslSocket>
#include <QMap>

#include "config.h"
#include "Session.h"
#include "protocol.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server();
    //funkcija gre čez vse Session in poišče client ki ima ta session, vrne session.
    bool containsSocket(QTcpSocket* socket);
    Session* getSession(QTcpSocket* clientSocket) const;
    //Session clientHasSession();
    ~Server() override;

public slots:
    // Slots that we connect to signals
    void onNewConnection();
    void onReadReady();
    void onDisconnected();


private:
    int authenticate(QTcpSocket *client, const QByteArray& username, const QByteArray& password);

    QTcpServer* server;

    QMap<QTcpSocket*, Session*> clients;
};

#endif // SERVER_H

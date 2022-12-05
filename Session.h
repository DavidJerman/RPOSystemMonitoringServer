//
// Created by david on 12/5/2022.
//

#ifndef SYSTEMMONITORINGSERVER_SESSION_H
#define SYSTEMMONITORINGSERVER_SESSION_H

#include <QTcpSocket>

class Session {
public:
    Session();

private:
    // Create a session with a client socket, client id and user id
    QTcpSocket *client;
    int clientID;
    int userID;
};


#endif //SYSTEMMONITORINGSERVER_SESSION_H

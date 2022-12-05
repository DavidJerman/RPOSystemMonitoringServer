//
// Created by david on 12/5/2022.
//

#ifndef SYSTEMMONITORINGSERVER_SESSION_H
#define SYSTEMMONITORINGSERVER_SESSION_H

#include <QTcpSocket>

class Session {
public:
    Session();

    virtual ~Session();

private:
    // Create a session with a client socket, client id and user id
    int clientID;
    int userID;
    bool validConnection;
};


#endif //SYSTEMMONITORINGSERVER_SESSION_H

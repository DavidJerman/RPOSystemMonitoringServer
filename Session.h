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

    [[nodiscard]] bool isAuthorized() const;

    [[nodiscard]] bool isAuthenticated() const;

    [[nodiscard]] int getClientId() const;

    void setClientId(int clientId);

    [[nodiscard]] int getUserId() const;

    void setUserId(int userId);

private:
    // Create a session with a client socket, client id and user id
    int clientID;
    int userID;
};


#endif //SYSTEMMONITORINGSERVER_SESSION_H

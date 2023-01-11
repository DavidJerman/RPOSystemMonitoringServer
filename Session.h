//
// Created by david on 12/5/2022.
//

#ifndef SYSTEMMONITORINGSERVER_SESSION_H
#define SYSTEMMONITORINGSERVER_SESSION_H

#include <QTcpSocket>
#include "components/system.h"

class Session {
public:
    Session();

    virtual ~Session();

    [[nodiscard]] bool isIdentified() const;

    [[nodiscard]] bool isAuthenticated() const;

    [[nodiscard]] int getClientId() const;

    void setClientId(int clientId);

    [[nodiscard]] int getUserId() const;

    void setUserId(int userId);

    [[nodiscard]] bool isSystemAdded() const;

    void setSystemAdded(bool _systemAdded);

    void addSystem(System *_system);

    [[nodiscard]] System *getSystem() const;

private:
    // Create a session with a client socket, client id and user id
    int clientID;
    int userID;
    bool systemAdded;
    System *system;
};


#endif //SYSTEMMONITORINGSERVER_SESSION_H

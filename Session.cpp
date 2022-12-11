//
// Created by david on 12/5/2022.
//

#include "Session.h"

Session::Session() {
    clientID = 0;
    userID = 0;
    systemAdded = false;
}

Session::~Session() = default;

bool Session::isAuthorized() const {
    return clientID != 0;
}

bool Session::isAuthenticated() const {
    return userID != 0;
}

int Session::getClientId() const {
    return clientID;
}

void Session::setClientId(int clientId) {
    clientID = clientId;
}

int Session::getUserId() const {
    return userID;
}

void Session::setUserId(int userId) {
    userID = userId;
}

bool Session::isSystemAdded() const {
    return systemAdded;
}

void Session::setSystemAdded(bool _systemAdded) {
    this->systemAdded = _systemAdded;
}

void Session::addSystem(System *_system) {
    this->system = _system;
}

System *Session::getSystem() const {
    return system;
}

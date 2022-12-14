#include "network.h"

#include <utility>

Network::Network(QString type, int fk_client, int ID, const QString &name) :
        Component(ID, name),
        type(std::move(type)),
        fkClient(fk_client) {}

QString Network::getType() const {
    return type;
}

void Network::setType(const QString &newType) {
    type = newType;
}

int Network::getFkClient() const {
    return fkClient;
}

void Network::setFkClient(int newFkClient) {
    fkClient = newFkClient;
}

#include "network.h"

Network::Network(const QString &type, int fk_client, int ID, const QString &name) :
    Component(ID, name),
    type(type),
    fk_client(fk_client)
{}

QString Network::getType() const
{
    return type;
}

void Network::setType(const QString &newType)
{
    type = newType;
}

int Network::getFk_client() const
{
    return fk_client;
}

void Network::setFk_client(int newFk_client)
{
    fk_client = newFk_client;
}

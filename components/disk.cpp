#include "disk.h"

double Disk::getCapacity() const
{
    return capacity;
}

void Disk::setCapacity(double newCapacity)
{
    capacity = newCapacity;
}

QString Disk::getType() const
{
    return type;
}

void Disk::setType(const QString &newType)
{
    type = newType;
}

int Disk::getFk_client() const
{
    return fk_client;
}

void Disk::setFk_client(int newFk_client)
{
    fk_client = newFk_client;
}

Disk::Disk(double capacity, const QString &type, int fk_client, int ID, const QString &name) :
    Component(ID, name),
    capacity(capacity),
    type(type),
    fk_client(fk_client)
{}

#include "ram.h"

Ram::Ram(double capacity, const QString &type, double frequency, int fk_client, int ID, const QString &name) :
    Component(ID, name),
    capacity(capacity),
    type(type),
    frequency(frequency),
    fk_client(fk_client)
{}

double Ram::getCapacity() const
{
    return capacity;
}

void Ram::setCapacity(double newCapacity)
{
    capacity = newCapacity;
}

QString Ram::getType() const
{
    return type;
}

void Ram::setType(const QString &newType)
{
    type = newType;
}

double Ram::getFrequency() const
{
    return frequency;
}

void Ram::setFrequency(double newFrequency)
{
    frequency = newFrequency;
}

int Ram::getFk_client() const
{
    return fk_client;
}

void Ram::setFk_client(int newFk_client)
{
    fk_client = newFk_client;
}

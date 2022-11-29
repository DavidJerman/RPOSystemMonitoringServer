#include "disk.h"

#include <utility>

double Disk::getCapacity() const {
    return capacity;
}

void Disk::setCapacity(double newCapacity) {
    capacity = newCapacity;
}

QString Disk::getType() const {
    return type;
}

void Disk::setType(const QString &newType) {
    type = newType;
}

int Disk::getFkClient() const {
    return fkClient;
}

void Disk::setFkClient(int newFkClient) {
    fkClient = newFkClient;
}

Disk::Disk(double capacity, QString type, int fkClient, int ID, const QString &name) :
        Component(ID, name),
        capacity(capacity),
        type(std::move(type)),
        fkClient(fkClient) {}

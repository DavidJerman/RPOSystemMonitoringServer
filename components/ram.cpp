#include "ram.h"

#include <utility>

Ram::Ram(double capacity, QString type, double frequency, int fkClient, int ID, const QString &name) :
        Component(ID, name),
        capacity(capacity),
        type(std::move(type)),
        frequency(frequency),
        fkClient(fkClient) {}

double Ram::getCapacity() const {
    return capacity;
}

void Ram::setCapacity(double newCapacity) {
    capacity = newCapacity;
}

QString Ram::getType() const {
    return type;
}

void Ram::setType(const QString &newType) {
    type = newType;
}

double Ram::getFrequency() const {
    return frequency;
}

void Ram::setFrequency(double newFrequency) {
    frequency = newFrequency;
}

int Ram::getFkClient() const {
    return fkClient;
}

void Ram::setFkClient(int newFkClient) {
    fkClient = newFkClient;
}

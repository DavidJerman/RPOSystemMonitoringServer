#include "cpu.h"

double Cpu::getMaxFrequency() const {
    return maxFrequency;
}

void Cpu::setMaxFrequency(double newMaxFrequency) {
    maxFrequency = newMaxFrequency;
}

int Cpu::getCores() const {
    return cores;
}

void Cpu::setCores(int newCores) {
    cores = newCores;
}

int Cpu::getFkClient() const {
    return fkClient;
}

void Cpu::setFkClient(int newFkClient) {
    fkClient = newFkClient;
}

Cpu::Cpu(double maxFrequency, int cores, int fkClient, int ID, const QString &name) :
        Component(ID, name),
        maxFrequency(maxFrequency),
        cores(cores),
        fkClient(fkClient) {}

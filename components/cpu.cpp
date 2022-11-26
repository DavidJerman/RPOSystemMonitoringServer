#include "cpu.h"

double Cpu::getMax_frequency() const
{
    return max_frequency;
}

void Cpu::setMax_frequency(double newMax_frequency)
{
    max_frequency = newMax_frequency;
}

int Cpu::getCores() const
{
    return cores;
}

void Cpu::setCores(int newCores)
{
    cores = newCores;
}

int Cpu::getFk_client() const
{
    return fk_client;
}

void Cpu::setFk_client(int newFk_client)
{
    fk_client = newFk_client;
}

Cpu::Cpu(double max_frequency, int cores, int fk_client, int ID, const QString &name) :
    Component(ID, name),
    max_frequency(max_frequency),
    cores(cores),
    fk_client(fk_client)
{}

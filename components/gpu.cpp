#include "gpu.h"

double Gpu::getMax_frequency() const
{
    return max_frequency;
}

void Gpu::setMax_frequency(double newMax_frequency)
{
    max_frequency = newMax_frequency;
}

double Gpu::getVram() const
{
    return vram;
}

void Gpu::setVram(double newVram)
{
    vram = newVram;
}

int Gpu::getFk_client() const
{
    return fk_client;
}

void Gpu::setFk_client(int newFk_client)
{
    fk_client = newFk_client;
}

Gpu::Gpu(double max_frequency, double vram, int fk_client, int ID, const QString &name) :
    Component(ID, name),
    max_frequency(max_frequency),
    vram(vram),
    fk_client(fk_client)
{}

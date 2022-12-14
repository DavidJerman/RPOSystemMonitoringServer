#include "gpu.h"

double Gpu::getMaxFrequency() const {
    return maxFrequency;
}

void Gpu::setMaxFrequency(double newMaxFrequency) {
    maxFrequency = newMaxFrequency;
}

double Gpu::getVRam() const {
    return vRam;
}

void Gpu::setVRam(double newVRam) {
    vRam = newVRam;
}

int Gpu::getFkClient() const {
    return fkClient;
}

void Gpu::setFkClient(int newFkClient) {
    fkClient = newFkClient;
}

Gpu::Gpu(double maxFrequency, double VRam, int fkClient, int ID, const QString &name) :
        Component(ID, name),
        maxFrequency(maxFrequency),
        vRam(VRam),
        fkClient(fkClient) {}

#ifndef GPU_H
#define GPU_H

#include "components/component.h"

class Gpu : public Component {
public:
    Gpu(double maxFrequency, double VRam, int fkClient, int ID, const QString &name);

    [[nodiscard]] double getMaxFrequency() const;

    void setMaxFrequency(double newMaxFrequency);

    [[nodiscard]] double getVRam() const;

    void setVRam(double newVRam);

    [[nodiscard]] int getFkClient() const;

    void setFkClient(int newFkClient);

private:
    double maxFrequency;
    double vRam;
    int fkClient;
};

#endif // GPU_H

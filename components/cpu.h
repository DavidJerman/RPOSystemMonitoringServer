#ifndef CPU_H
#define CPU_H

#include "components/component.h"

class Cpu : public Component {
public:
    Cpu(double maxFrequency, int cores, int fkClient, int ID, const QString &name);

    [[nodiscard]] double getMaxFrequency() const;

    void setMaxFrequency(double newMaxFrequency);

    [[nodiscard]] int getCores() const;

    void setCores(int newCores);

    [[nodiscard]] int getFkClient() const;

    void setFkClient(int newFkClient);

private:
    double maxFrequency;
    int cores;
    int fkClient;
};

#endif // CPU_H

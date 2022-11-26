#ifndef CPU_H
#define CPU_H

#include "components/component.h"

class Cpu : public Component
{
public:
    Cpu(double max_frequency, int cores, int fk_client, int ID, const QString &name);

    double getMax_frequency() const;
    void setMax_frequency(double newMax_frequency);

    int getCores() const;
    void setCores(int newCores);

    int getFk_client() const;
    void setFk_client(int newFk_client);

private:
    double max_frequency;
    int cores;
    int fk_client;
};

#endif // CPU_H

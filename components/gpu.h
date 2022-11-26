#ifndef GPU_H
#define GPU_H

#include "components/component.h"

class Gpu : public Component
{
public:
    Gpu(double max_frequency, double vram, int fk_client, int ID, const QString &name);


    double getMax_frequency() const;
    void setMax_frequency(double newMax_frequency);

    double getVram() const;
    void setVram(double newVram);

    int getFk_client() const;
    void setFk_client(int newFk_client);

private:
    double max_frequency;
    double vram;
    int fk_client;
};

#endif // GPU_H

#ifndef RAM_H
#define RAM_H

#include "components/component.h"

class Ram : public Component
{
public:
    Ram(double capacity, const QString &type, double frequency, int fk_client, int ID, const QString &name);


    double getCapacity() const;
    void setCapacity(double newCapacity);
    QString getType() const;
    void setType(const QString &newType);
    double getFrequency() const;
    void setFrequency(double newFrequency);
    int getFk_client() const;
    void setFk_client(int newFk_client);

private:
    double capacity;
    QString type;
    double frequency;
    int fk_client;
};

#endif // RAM_H

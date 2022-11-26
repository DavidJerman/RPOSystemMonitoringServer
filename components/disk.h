#ifndef DISK_H
#define DISK_H

#include "components/component.h"
#include <QString>

class Disk : public Component
{
public:
    Disk(double capacity, const QString &type, int fk_client, int ID, const QString &name);

    double getCapacity() const;
    void setCapacity(double newCapacity);

    QString getType() const;
    void setType(const QString &newType);

    int getFk_client() const;
    void setFk_client(int newFk_client);

private:
    double capacity;
    QString type;
    int fk_client;
};

#endif // DISK_H

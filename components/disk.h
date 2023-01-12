#ifndef DISK_H
#define DISK_H

#include "components/component.h"
#include <QString>

#endif // DISK_H

class Disk : public Component {
public:
    Disk(double capacity, QString type, int fkClient, int ID, const QString &name);

    [[nodiscard]] double getCapacity() const;

    void setCapacity(double newCapacity);

    [[nodiscard]] QString getType() const;

    void setType(const QString &newType);

    [[nodiscard]] int getFkClient() const;

    void setFkClient(int newFkClient);

private:
    double capacity;
    QString type;
    int fkClient;
};



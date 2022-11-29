#ifndef RAM_H
#define RAM_H

#include "components/component.h"

class Ram : public Component {
public:
    Ram(double capacity, QString type, double frequency, int fkClient, int ID, const QString &name);

    [[nodiscard]] double getCapacity() const;

    void setCapacity(double newCapacity);

    [[nodiscard]] QString getType() const;

    void setType(const QString &newType);

    [[nodiscard]] double getFrequency() const;

    void setFrequency(double newFrequency);

    [[nodiscard]] int getFkClient() const;

    void setFkClient(int newFkClient);

private:
    double capacity;
    QString type;
    double frequency;
    int fkClient;
};

#endif // RAM_H

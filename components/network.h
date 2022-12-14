#ifndef NETWORK_H
#define NETWORK_H

#include "components/component.h"

class Network : public Component {
public:
    Network(QString type, int fk_client, int ID, const QString &name);

    [[nodiscard]] QString getType() const;

    void setType(const QString &newType);

    [[nodiscard]] int getFkClient() const;

    void setFkClient(int newFkClient);

private:
    QString type;
    int fkClient;
};

#endif // NETWORK_H

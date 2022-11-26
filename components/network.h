#ifndef NETWORK_H
#define NETWORK_H

#include "components/component.h"

class Network : public Component
{
public:
    Network(const QString &type, int fk_client, int ID, const QString &name);


    QString getType() const;
    void setType(const QString &newType);
    int getFk_client() const;
    void setFk_client(int newFk_client);

private:
    QString type;
    int fk_client;
};

#endif // NETWORK_H

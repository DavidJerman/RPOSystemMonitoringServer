#ifndef SYSTEM_H
#define SYSTEM_H

#include "components/cpu.h"
#include "components/disk.h"
#include "components/gpu.h"
#include "components/network.h"
#include "components/ram.h"

#include <QList>

class System
{
public:
    System();
    System(const QList<Component *> &components);

    ~System();

    void addComponent(Component* component);

    QList<Component*>::const_iterator begin() const;

    QList<Component*>::const_iterator end() const;

    bool hasComponent(Component *component) const;

    void removeComponent(Component* component);

    QList<Component*> getComponents() const;

private:
    QList<Component*> components;
};

#endif // SYSTEM_H

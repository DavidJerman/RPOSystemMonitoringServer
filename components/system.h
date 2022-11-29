#ifndef SYSTEM_H
#define SYSTEM_H

#include "components/cpu.h"
#include "components/disk.h"
#include "components/gpu.h"
#include "components/network.h"
#include "components/ram.h"

#include <QList>

class System {
public:
    System();

    explicit System(const QList<Component *> &components);

    ~System();

    void addComponent(Component *component);

    [[nodiscard]] QList<Component *>::const_iterator begin() const;

    [[nodiscard]] QList<Component *>::const_iterator end() const;

    bool hasComponent(Component *component) const;

    void removeComponent(Component *component);

    [[nodiscard]] QList<Component *> getComponents() const;

    static bool isCpu(Component *component);

    static bool isGpu(Component *component);

    static bool isDisk(Component *component);

    static bool isNetwork(Component *component);

    static bool isRam(Component *component);

private:
    QList<Component *> components;
};

#endif // SYSTEM_H

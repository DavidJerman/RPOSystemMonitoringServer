#include "system.h"

System::System() = default;

System::System(const QList<Component *> &components) :
        components(components) {}

System::~System() {
    for (auto component: components) {
        delete component;
    }
}

void System::addComponent(Component *component) {
    if (component != nullptr)
        components.append(component);
}

QList<Component *>::const_iterator System::begin() const {
    return components.begin();
}

QList<Component *>::const_iterator System::end() const {
    return components.cend();
}

bool System::hasComponent(Component *component) const {
    return components.contains(component);
}

void System::removeComponent(Component *component) {
    components.removeAt(components.indexOf(component));
}

QList<Component *> System::getComponents() const {
    return components;
}

bool System::isCpu(Component *const component) {
    return dynamic_cast<Cpu *> (component);
}

bool System::isGpu(Component *const component) {
    return dynamic_cast<Gpu *> (component);
}

bool System::isDisk(Component *const component) {
    return dynamic_cast<Disk *> (component);
}

bool System::isNetwork(Component *const component) {
    return dynamic_cast<Network *> (component);
}

bool System::isRam(Component *const component) {
    return dynamic_cast<Ram *> (component);
}

void System::sort() {
    // Add all the rams at the beginning of the list
    for (int i = 0; i < components.size(); i++) {
        if (isRam(components[i])) {
            components.move(i, 0);
        }
    }
    // Add all the networks at the beginning of the list
    for (int i = 0; i < components.size(); i++) {
        if (isNetwork(components[i])) {
            components.move(i, 0);
        }
    }
    // Add all the gpus at the beginning of the list
    for (int i = 0; i < components.size(); i++) {
        if (isGpu(components[i])) {
            components.move(i, 0);
        }
    }
    // Add all the disk at the beginning of the list
    for (int i = 0; i < components.size(); i++) {
        if (isDisk(components[i])) {
            components.move(i, 0);
        }
    }
    // Add all cpus at the beginning of the list
    for (int i = 0; i < components.size(); i++) {
        if (isCpu(components[i])) {
            components.move(i, 0);
        }
    }
}

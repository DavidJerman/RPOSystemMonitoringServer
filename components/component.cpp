#include "component.h"

Component::~Component() = default;

int Component::getId() const {
    return ID;
}

void Component::setId(int newId) {
    ID = newId;
}

QString Component::getName() const {
    return name;
}

void Component::setName(const QString &newName) {
    name = newName;
}

Component::Component(int id, const QString &name) :
        ID(id),
        name(name) {}

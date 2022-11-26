#include "component.h"

int Component::getID() const
{
    return ID;
}

void Component::setID(int newID)
{
    ID = newID;
}

QString Component::getName() const
{
    return name;
}

void Component::setName(const QString &newName)
{
    name = newName;
}

Component::Component(int ID, const QString &name) :
    ID(ID),
    name(name)
{}

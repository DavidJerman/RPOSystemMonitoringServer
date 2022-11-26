#include "system.h"

System::System()
{}

System::System(const QList<Component *> &components) :
    components(components)
{}

System::~System()
{
    for (auto component : components)
    {
        delete component;
    }
}

void System::addComponent(Component *component)
{
    if (component != nullptr)
        components.append(component);
}

QList<Component *>::const_iterator System::begin() const
{
    return components.begin();
}

QList<Component *>::const_iterator System::end() const
{
    return components.cend();
}

bool System::hasComponent(Component *component) const
{
    return components.contains(component);
}

void System::removeComponent(Component *component)
{
    components.remove(components.indexOf(component));
}

QList<Component*> System::getComponents() const
{
    return components;
}

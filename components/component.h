#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

class Component {
public:
    Component(int ID, const QString &name);

    virtual ~Component();

    [[nodiscard]] int getID() const;

    void setID(int newID);

    [[nodiscard]] QString getName() const;

    void setName(const QString &newName);

private:
    int ID;
    QString name;
};

#endif // COMPONENT_H

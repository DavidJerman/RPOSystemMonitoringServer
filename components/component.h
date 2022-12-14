#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

class Component {
public:
    Component(int id, const QString &name);

    virtual ~Component();

    [[nodiscard]] int getId() const;

    void setId(int newId);

    [[nodiscard]] QString getName() const;

    void setName(const QString &newName);

private:
    int ID;
    QString name;
};

#endif // COMPONENT_H

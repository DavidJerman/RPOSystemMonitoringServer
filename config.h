#ifndef CONFIG_H
#define CONFIG_H


#include <QMap>
#include <QString>
#include <QFile>

#include <QException>


class Config
{
public:
    Config() = default;

    static void loadDataFromFile(QString fileName);

    static void addProperty(QString property, QString value);

    static QString getValue(QString property);

    static bool isEmpty();

    static bool hasProperty(QString property);

    static bool hasValue(QString value);

    static qsizetype size();

    static void clear();

private:
    static bool isDataLoaded;

    static QMap<QString, QString> config;
};


// NEEDS TO BE TESTED WITH QT CODE!
class PropertyNotFoundException : public QException
{
public:
    PropertyNotFoundException(const QString &_message = "") noexcept
        : _message(_message) {}


    void raise() const override
    {
        throw *this;
    }


    PropertyNotFoundException *clone() const override
    {
        return new PropertyNotFoundException(*this);
    }


    QString message() const noexcept
    {
        return _message;
    }

private:
    QString _message;
};


#endif // CONFIG_H

#ifndef CONFIG_H
#define CONFIG_H


#include <QMap>
#include <QString>
#include <QFile>
#include <QVector>

#include <QException>


class Config
{
public:
    Config() = default;

    static void loadDataFromFile(QString fileName);

    static void addProperty(QString property, QString value);

    static QString getValue(QString property);

    static const QList<QString> getProperties();

    static bool isEmpty() noexcept;

    static bool hasProperty(QString property);

    static bool hasValue(QString value);

    static qsizetype size() noexcept;

    static void clear() noexcept;

private:
    static bool isDataLoaded;

    static QMap<QString, QString> config;


private:
    // Private functions for string data manipulation
    static QVector<QByteArray> split(QByteArray &data, char delimiter);

    static QByteArray join(QVector<QByteArray> &vector);

    static QByteArray &remove(QByteArray &data, char ch);
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


class FileException : public QException
{
public:
    FileException(const QString &_message = "") noexcept
        : _message(_message) {}


    void raise() const override
    {
        throw *this;
    }


    FileException *clone() const override
    {
        return new FileException(*this);
    }


    QString message() const noexcept
    {
        return _message;
    }

private:
    QString _message;
};



#endif // CONFIG_H

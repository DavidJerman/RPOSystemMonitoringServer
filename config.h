#ifndef CONFIG_H
#define CONFIG_H


#include <QMap>
#include <QString>
#include <QFile>
#include <QVector>

#include <QException>


class Config {
public:
    Config();

    void loadFromFile(const QString& fileName);

    void addProperty(QString property, QString value);

    QString getProperty(QString property);

    QList<QString> getProperties();

    bool isEmpty() noexcept;

    bool hasProperty(const QString& property);

    bool hasValue(const QString& value);

    bool isLoaded();

    qsizetype size() noexcept;

    void clear() noexcept;

private:
    QMap<QString, QString> config;


private:
    // Private functions for string data manipulation
    static QVector<QByteArray> split(QByteArray &data, char delimiter);

    static QByteArray join(QVector<QByteArray> &vector);

    static QByteArray &remove(QByteArray &data, char value);
};


// NEEDS TO BE TESTED WITH QT CODE!
class PropertyNotFoundException : public QException {
public:
    PropertyNotFoundException(const QString &_message = "") noexcept
            : _message(_message) {}


    void raise() const override {
        throw *this;
    }


    PropertyNotFoundException *clone() const override {
        return new PropertyNotFoundException(*this);
    }


    QString message() const noexcept {
        return _message;
    }

private:
    QString _message;
};


class FileException : public QException {
public:
    FileException(const QString &_message = "") noexcept
            : _message(_message) {}


    void raise() const override {
        throw *this;
    }


    FileException *clone() const override {
        return new FileException(*this);
    }


    QString message() const noexcept {
        return _message;
    }

private:
    QString _message;
};


#endif // CONFIG_H

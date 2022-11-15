#include "config.h"


bool Config::isDataLoaded;


QMap<QString, QString> Config::config;


// Upon exception, no data is loaded
void Config::loadDataFromFile(QString fileName)
{
    QFile file (fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        auto message = QString("Error openning file " + fileName + ": " + file.errorString());
        throw FileException(message);
    }

    while (!file.atEnd()) {
        // Config file style
        QByteArray line = file.readLine();
        // These symbols will be ignored in the config file
        remove(line, '\n');
        remove(line, '\r');
        remove(line, ' ');

        auto data = split(line, ':');

        auto &property = data[0];
        auto &value = data[1];

        // Invalid file format exception
        if (data.size() != 2)
        {
            clear();
            throw FileException("Invalid file format!");
        }
        // Duplicate config entries warning
        if (hasProperty(property))
        {
            clear();
            throw FileException("Duplicate entry found in config file: " + property);
        }

        addProperty(property, value);
    }
}


void Config::addProperty(QString property, QString value)
{
    config.insert(property, value);
}


QString Config::getValue(QString property)
{
    auto key = config.find(property);
    if (key == config.end())
    {
        auto message = QString("Property " + property + " not found!");
        throw PropertyNotFoundException(message);
    }
    return key.value();
}


const QList<QString> Config::getProperties()
{
    return config.keys();
}


bool Config::isEmpty() noexcept
{
    return config.isEmpty();
}


bool Config::hasProperty(QString property)
{
    return config.contains(property);
}


bool Config::hasValue(QString value)
{
    for (auto &_value : config.values())
    {
        if (QString::compare(_value, value) == 0)
        {
            return true;
        }
    }
    return false;
}


qsizetype Config::size() noexcept
{
    return config.size();
}


void Config::clear() noexcept
{
    config.clear();
}


// This function should work without exceptions
QVector<QByteArray> Config::split(QByteArray &data, char delimiter)
{
    QVector<QByteArray> array;
    for (int start = 0, end = 0; end <= data.size(); end++)
    {
        if (end == data.size()) {
            if (start < end && start < data.size())
            {
                array.append(data.mid(start, data.size() - start));
            }
            break;
        }
        if (data[end] == delimiter)
        {
            if (start - end != 0)
                array.append(data.mid(start, end - start));
            start = end + 1;
        }
    }
    return std::move(array);
}


QByteArray Config::join(QVector<QByteArray> &vector)
{
    QByteArray data;
    for (auto date : vector)
    {
        data.append(date);
    }
    return data;
}


QByteArray &Config::remove(QByteArray &data, char ch)
{
    auto splitData = split(data, ch);
    data = join(splitData);
    return data;
}

#include "config.h"


bool Config::isDataLoaded;


QMap<QString, QString> Config::config;


void Config::loadDataFromFile(QString fileName)
{

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


bool Config::isEmpty()
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


qsizetype Config::size()
{
    return config.size();
}


void Config::clear()
{
    config.clear();
}

#include "config.h"


/**
 * Initializes private variables and constructs the class.
 * @brief Config constructor
 */
Config::Config()
        : config(QMap<QString, QString>()) {

}


/**
 * Loads the configuration from a file. If an error is encoutered FileException is raised.
 * Errors include: invalid file format, duplicate property in configuration file.
 * @brief Loads data from file
 * @param fileName Configuration file name
 * @throws FileException
 */
void Config::loadFromFile(QString fileName) {
    QFile file(fileName);

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
        // For now spaces are not supported in the config file
        // TODO: Add support for this
        remove(line, ' ');

        auto data = split(line, ':');

        auto &property = data[0];
        auto &value = data[1];

        // Invalid file format exception
        if (data.size() != 2) {
            clear();
            throw FileException("Invalid file format!");
        }
        // Duplicate config entries warning
        if (hasProperty(property)) {
            clear();
            throw FileException("Duplicate entry found in config file: " + property);
        }

        addProperty(property, value);
    }
}


/**
 * @brief Adds a new property to the configuration
 * @param property New Property
 * @param value Property value
 */
void Config::addProperty(QString property, QString value) {
    config.insert(property, value);
}


/**
 * @brief Returns property value
 * @param property Property
 * @return Property value
 */
QString Config::getValue(QString property) {
    auto key = config.find(property);
    if (key == config.end()) {
        auto message = QString("Property " + property + " not found!");
        throw PropertyNotFoundException(message);
    }
    return key.value();
}


/**
 * Returns a copy of all configuration properties as a QList.
 * @brief Returns configuration properties
 * @return List of properties
 */
const QList<QString> Config::getProperties() {
    return config.keys();
}


/**
 * @brief Is the configuration empty
 * @return Is empty
 */
bool Config::isEmpty() noexcept {
    return config.isEmpty();
}


/**
 * @brief Checks if the configuration has a given property
 * @param property Property
 * @return Has property
 */
bool Config::hasProperty(QString property) {
    return config.contains(property);
}


/**
 * @brief Checks if the configuration has a given value
 * @param value Value
 * @return Has value
 */
bool Config::hasValue(QString value) {
    for (auto &_value: config.values()) {
        if (QString::compare(_value, value) == 0) {
            return true;
        }
    }
    return false;
}


/**
 * @brief Chechks if the configuration is loaded
 * @return Is loaded
 */
bool Config::isLoaded() {
    return (bool) size();
}


/**
 * @brief Returns the number of properties in the configuration
 * @return Number of properties
 */
qsizetype Config::size() noexcept {
    return config.size();
}


/**
 * @brief Removes all properties from the documentation
 */
void Config::clear() noexcept {
    config.clear();
}


/**
 * Splits the byte array into multiple arrays with the specified delimiter
 * and returns the split array as a vector of arrays.
 * @brief Splits the QByteArray
 * @param data QByteArray to split
 * @param delimiter Delimiter for splitting
 * @return Vector of the split array
 */
QVector<QByteArray> Config::split(QByteArray &data, char delimiter) {
    QVector<QByteArray> array;
    for (int start = 0, end = 0; end <= data.size(); end++) {
        if (end == data.size()) {
            if (start < end && start < data.size()) {
                array.append(data.mid(start, data.size() - start));
            }
            break;
        }
        if (data[end] == delimiter) {
            if (start - end != 0)
                array.append(data.mid(start, end - start));
            start = end + 1;
        }
    }
    return std::move(array);
}


/**
 * @brief Joins together multiple byte arrays
 * @param vector Vector of byte arrays
 * @return Joint array
 */
QByteArray Config::join(QVector<QByteArray> &vector) {
    QByteArray data;
    for (auto date: vector) {
        data.append(date);
    }
    return data;
}


/**
 * @brief Removes the specified values from the byte array
 * @param data Array
 * @param value Value to remove
 * @return Modified array
 */
QByteArray &Config::remove(QByteArray &data, char value) {
    auto splitData = split(data, value);
    data = join(splitData);
    return data;
}

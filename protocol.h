#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonDocument>

#include <QException>

/**
 * This class defines the protocol.
 * The structure used for sending data is json.
 * The protocol includes identifying the following messages:
 * - authentication,
 * - add new system,
 * - add new data.
 * @brief The Protocol class
 */
class Protocol
{
public:
    Protocol();

    static QJsonDocument jsobObjectFromFile(QString file);

    static QString jsonStringFromFile(QString file);


};

#endif // PROTOCOL_H

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonDocument>

#include <QException>

#include "components/system.h"

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

    static QJsonDocument jsobDocumentFromFile(QString file);

    static QString jsonStringFromFile(QString file);

    static System* jsonDocumentToSystem(const QJsonDocument& doc);
};


// Data containing classes



#endif // PROTOCOL_H

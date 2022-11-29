#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QException>

#include <QFile>

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
class Protocol {
public:
    Protocol();

    static QByteArray UTF8JsonFromFile(const QString& fileName);

    static QJsonDocument UTF8ToJsonDocument(const QByteArray &data);

    static QByteArray jsonDocumentToUTF8(const QJsonDocument &data);

    static System *jsonDocumentToSystem(const QJsonDocument &doc);

    static QJsonDocument systemToJsonDocument(System *system);
};


#endif // PROTOCOL_H

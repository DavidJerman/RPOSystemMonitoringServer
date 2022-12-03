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

    static void UTF8JsonToFile(QByteArray& json, const QString& fileName);

    static QJsonDocument UTF8ToJsonDocument(const QByteArray &data);

    static QByteArray jsonDocumentToUTF8(const QJsonDocument &data);

    static System *jsonDocumentToSystem(const QJsonDocument &doc);

    static QJsonDocument systemToJsonDocument(System *system);

    static QByteArray getUsernameFromJson(QByteArray &json);

    static QByteArray getPasswordFromJson(QByteArray &json);

    static QByteArray getAuthenticationJson(QByteArray &username, QByteArray &password);

    static int getClientIDFromJson(QByteArray &json);

    static QByteArray getClientIdentificationJson(int &clientID);

    static QByteArray getConfirmationJson(bool &confirmation);

    // TODO: Add data to json methods
};


#endif // PROTOCOL_H

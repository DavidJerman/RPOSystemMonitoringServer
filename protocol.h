#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QException>

#include <QFile>

#include "components/system.h"

enum MESSAGE {
    UNKNOWN,
    AUTH,
    ID,
    SYSTEM,
    DATA,
    CONFIRM
};

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

    static QByteArray UTF8JsonFromFile(const QString &fileName);

    static void UTF8JsonToFile(QByteArray &json, const QString &fileName);

    static System *jsonToSystem(const QByteArray &json);

    static QByteArray systemToJson(System *system);

    static QByteArray getUsername(QByteArray &json);

    static QByteArray getPassword(QByteArray &json);

    static QByteArray getAuthenticationJson(QByteArray &username, QByteArray &password);

    static int getClientId(QByteArray &json);

    static QByteArray getClientIdentificationJson(int &clientID);

    static QByteArray getConfirmationJson(bool confirmation);

    static QByteArray getErrorJson(QString &error);

    static QByteArray jsonToMessage(MESSAGE messageType, const QByteArray &data);

    static QList<MESSAGE> getMessageType(const QByteArray &message);

    static QList<QByteArray> messageToJson(const QByteArray &message);
};


#endif // PROTOCOL_H

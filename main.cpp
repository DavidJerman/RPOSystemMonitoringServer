#include <QCoreApplication>

#include "server.h"
#include "protocol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    auto data = Protocol::jsonToMessage(AUTH, "Hello World");
    Protocol::messageToJson(data);
    Protocol::getMessageType(data);

    return QCoreApplication::exec();
}

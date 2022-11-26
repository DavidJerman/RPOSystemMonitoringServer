#include <QCoreApplication>

#include "server.h"
#include "protocol.h"

int main(int argc, char *argv[])
{
     QCoreApplication a(argc, argv);

     // Server server;

     auto data = Protocol::UTF8JsonFromFile("jsonExamples/newUpdateSystem.json");
     auto system = Protocol::jsonDocumentToSystem(Protocol::UTF8ToJsonDocument(data));

     return QCoreApplication::exec();
}

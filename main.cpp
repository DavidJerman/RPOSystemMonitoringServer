#include <QCoreApplication>

#include "server.h"
#include "protocol.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Server server;

    auto data = Protocol::UTF8JsonFromFile(R"(C:\QtBuilds\build-SystemMonitoringServer-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\jsonExamples\newUpdateSystem.json)");

    auto doc = Protocol::UTF8ToJsonDocument(data);
    auto system = Protocol::jsonDocumentToSystem(doc);

    return QCoreApplication::exec();
}

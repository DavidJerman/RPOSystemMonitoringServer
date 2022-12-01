#include <QCoreApplication>

#include "server.h"
#include "protocol.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Server server;

    auto data = Protocol::UTF8JsonFromFile(R"(C:\QtBuilds\build-SystemMonitoringServer-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\jsonExamples\newUpdateSystem.json)");

    auto doc = Protocol::UTF8ToJsonDocument(data);
    auto system = Protocol::jsonDocumentToSystem(doc);
    auto doc2 = Protocol::systemToJsonDocument(system);
    auto text = Protocol::jsonDocumentToUTF8(doc2);
    Protocol::UTF8JsonToFile(text, R"(C:\QtBuilds\build-SystemMonitoringServer-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\jsonExamples\test.json)");

    return QCoreApplication::exec();
}

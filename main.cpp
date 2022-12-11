#include <QCoreApplication>

#include "server.h"
#include "protocol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;



    return QCoreApplication::exec();
}

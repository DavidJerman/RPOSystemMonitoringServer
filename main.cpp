#include <QCoreApplication>

#include "server.h"

// Temp
#include "config.h"
#include <iostream>

int main(int argc, char *argv[])
{

     QCoreApplication a(argc, argv);

     Server server;

     return a.exec();
}

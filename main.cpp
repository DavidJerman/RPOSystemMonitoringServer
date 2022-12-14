#include <QCoreApplication>
#include <QThread>
#include <unistd.h>

#include "server.h"
#include "protocol.h"
#include "tests/run_tests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    // Run tests in separate thread
    auto* thread = new QThread();
    QObject::connect(thread, &QThread::started, []() {
        assert(run_tests() == true);
    });
    thread->start();

    return QCoreApplication::exec();
}

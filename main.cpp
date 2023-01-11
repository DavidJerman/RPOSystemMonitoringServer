#include <QCoreApplication>
#include <QThread>

#include "server.h"
#include "tests/run_tests.h"

#include "components/data.h"

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

    // For testing purposes
    auto partNumbers = RAM::getPartNumbers();
    for (const auto &partNumber: partNumbers) {
        qDebug() << partNumber;
    }

    return QCoreApplication::exec();
}

#include <QCoreApplication>
#include <QThread>
#include <unistd.h>

#include "server.h"
#include "protocol.h"
#include "tests/run_tests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

// Run tests in separate thread
    auto* thread = new QThread();
    QObject::connect(thread, &QThread::started, []() {
        //assert(run_tests() == true);
        Server server;
        //QByteArray ime;
        char IME = 'Nino';
        //std:: string i = IME;
        std::string i = "Nino";
        QString strIme = QString::fromStdString(i);
        //QString strIme = "Nino";
        QByteArray ime = strIme.toUtf8();
        //ime.toStdString() = "Aljaž";
        qDebug() << "ime: " << ime;




        //QByteArray password;

        QString strPwd = "ninopassword";
        QByteArray password = strPwd.toUtf8();
        //password.toStdString() = "thisispasword";
        //qDebug() << "password: " << password;

        //server.authenticate(ime, password);
        server.authenticate1(ime);
    });
    thread->start();

//FOR RUNNING WITH TESTS, FAIL IF 1 test error
/*
    // Run tests in separate thread
    auto* thread = new QThread();
    QObject::connect(thread, &QThread::started, []() {
        assert(run_tests() == true);
    });
    thread->start();
*/
    return QCoreApplication::exec();
}

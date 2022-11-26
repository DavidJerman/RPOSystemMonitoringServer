#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

#include <memory>

class Server : public QObject
{
    Q_OBJECT
public:
    //Server();
    explicit Server(QObject *parent = 0);

    //virtual ~Server();
    ~Server();

public slots:
    // Slots that we connect to signals
    void onNewConnection();

    void onReadReady();
    void onDisconnected();


private:
    QTcpServer* server;

    QList<QTcpSocket*> clients;
};

#endif // SERVER_H

/*
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QTimer>

#include <QString>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>
#include <QFileDialog>
#include <memory>

//#include <QMetaType>
//#include <QSet>
#include <QMainWindow> //A main window provides a framework for building an application's user interface
                       //https://doc.qt.io/qt-6/qmainwindow.html#details

//qDebug() <<

namespace UI {  //User interface
    class Server;
}

class Server : public QMainWindow {
    Q_OBJECT
private:
    UI::Server *ui;

    QTcpServer* server;
    QList<QTcpSocket*> clients;


public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

signals:

    void newMessage(QString);
public slots:
    // Slots that we connect to signals

    void onNewConnection();
    void onReadReady();

     void readSocket();


};
*/

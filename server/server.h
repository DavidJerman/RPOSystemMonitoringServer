#ifndef SERVER_H
#define SERVER_H

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



//qDebug() <<
class Server : public QObject {
private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    //Server();

    ~Server();

signals:

    void newMessage(QString);
public slots:
    // Slots that we connect to signals

    void onNewConnection();
    void onReadReady();

     void readSocket();


};

#endif // SERVER_H

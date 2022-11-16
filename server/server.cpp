#include "server.h"

/**
 * @brief Constructs the server and loads the config
 */
// Load config here
Server::Server(QObject *parent)
    : QObject{parent}
{
    // Load config here

    // TODO: Create QTcpServer properly
    server = new QTcpServer();

/*  //Davidov del za naredit
    // Temp
    // Just for testing
    Config config;

    try {
        config.getValue("Test");

    }
    catch (const PropertyNotFoundException& e)
    {
        qDebug() << e.message();
    }

    try {
        config.loadFromFile("test.txt");
    }
    catch (const FileException& e)
    {
        qDebug() << e.message();
    }

    auto properties = config.getProperties();
    for (auto &property : properties)
    {
        qDebug() << property << config.getValue(property);
    }

*/
}

/**
 * @brief Deletes the server and disconnectes all clients
 */
Server::~Server()
{
    server->close();
    server->deleteLater();

    // TODO: Disconnect all clients
  /*  for (auto client : clients) {
        // TODO
    } */
}


/**
 * @brief Accepts the new connection and saves it to a list
 */
void Server::onNewConnection()
{
    // Add new client to the list
    auto client = server->nextPendingConnection();
    clients.append(client);
}


/**
 * @brief Reads the client message and processes it
 */
void Server::onReadReady()
{
    // Accept
}

void Server::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;      //velikost sporočila

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_5);

    socketStream.startTransaction();    // začetek novega branja transakcije
    socketStream >> buffer;

    if(!socketStream.commitTransaction())       //konča s branjem transakcije, vrača true, če brez napak, sicer false
    {
        QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());     // čakanje kode
        emit newMessage(message);
        return;
    }

    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];

    buffer = buffer.mid(128);

    if(fileType=="attachment"){
        QString fileName = header.split(",")[1].split(":")[1];
        QString ext = fileName.split(".")[1];
        QString size = header.split(",")[2].split(":")[1].split(";")[0];

        if (QMessageBox::Yes == QMessageBox::question(this, "QTCPServer", QString("You are receiving an attachment from sd:%1 of size: %2 bytes, called %3. Do you want to accept it?").arg(socket->socketDescriptor()).arg(size).arg(fileName)))
        {
            QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/"+fileName, QString("File (*.%1)").arg(ext));

            QFile file(filePath);
            if(file.open(QIODevice::WriteOnly)){
                file.write(buffer);
                QString message = QString("INFO :: Attachment from sd:%1 successfully stored on disk under the path %2").arg(socket->socketDescriptor()).arg(QString(filePath));
                emit newMessage(message);
            }else
                QMessageBox::critical(this,"QTCPServer", "An error occurred while trying to write the attachment.");
        }else{
            QString message = QString("INFO :: Attachment from sd:%1 discarded").arg(socket->socketDescriptor());
            emit newMessage(message);
        }
    }else if(fileType=="message"){
        QString message = QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        emit newMessage(message);
    }
}


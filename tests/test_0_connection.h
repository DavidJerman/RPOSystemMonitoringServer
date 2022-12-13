//
// Created by david on 13. 12. 2022.
//

#include <QDebug>
#include <QTcpSocket>
#include "protocol.h"
#include "Utils.h"

using namespace Utils;

namespace Test_0 {
    int run(const QString &address, int port) {
        // Connect QTcpSocket to server
        auto *socket = new QTcpSocket();
        socket->connectToHost(address, port);
        if (!socket->waitForConnected(3000)) {
            println("Failed to connect to server - timed out, test failed!", Color::RED);
            return 1;
        }
        if (!socket->isValid()) {
            println("Failed to connect to server - invalid socket, test failed!", Color::RED);
            return 1;
        }
        // Try sending data
        if (socket->isValid()) {
            socket->write(Protocol::createMessage(MESSAGE::CONFIRM, Protocol::getConfirmationJson(false)));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(3000);
            auto data = socket->readAll();
            println("Received data!", Color::BLUE);
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (confirm) {
                println("Confirmation is true, test failed!", Color::RED);
                return 1;
            } else {
                println("Confirmation is false!", Color::BLUE);
            }
        }
        // Close socket
        socket->close();
        if (socket->isValid()) {
            println("Socket is still valid, test failed!", Color::RED);
            return 1;
        }
        println("Test passed!", Color::BLUE);
        return 0;
    }
}
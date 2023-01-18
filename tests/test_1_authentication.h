//
// Created by david on 13. 12. 2022.
//

#include "Constants.h"

namespace Test_1 {
    int run(const QString &address, int port) {
        // Connect QTcpSocket to server
        auto *socket = new QTcpSocket();
        socket->connectToHost(address, port);
        if (!socket->waitForConnected(WAIT_FOR_READY_READ)) {
            println("Failed to connect to server - timed out, test failed!", Color::RED);
            return 1;
        }
        if (!socket->isValid()) {
            println("Failed to connect to server - invalid socket, test failed!", Color::RED);
            return 1;
        }
        // Try authenticating with wrong username and password
        if (socket->isValid()) {
            socket->write(Protocol::createMessage(MESSAGE::AUTHENTICATE,
                                                  Protocol::getAuthenticationJson("wrong_username", "wrong_password")));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            auto data = socket->readAll();
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation for wrong username and password!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (!confirm) {
                println("Authentication failed, test succeeded!", Color::BLUE);
            } else {
                println("Authentication succeeded, test failed!", Color::RED);
                return 1;
            }
        }
        // Close socket
        socket->close();

        println("==>", Color::WHITE);
        // Connect QTcpSocket to server
        socket = new QTcpSocket();
        socket->connectToHost(address, port);
        if (!socket->waitForConnected(WAIT_FOR_READY_READ)) {
            println("Failed to connect to server - timed out, test failed!", Color::RED);
            return 1;
        }
        if (!socket->isValid()) {
            println("Failed to connect to server - invalid socket, test failed!", Color::RED);
            return 1;
        }
        // Try authenticating with correct username and wrong password
        if (socket->isValid()) {
            socket->write(Protocol::createMessage(MESSAGE::AUTHENTICATE,
                                                  Protocol::getAuthenticationJson("test", "wrong_password")));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            auto data = socket->readAll();
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation for correct username and wrong password!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (!confirm) {
                println("Authentication failed, test succeeded!", Color::BLUE);
            } else {
                println("Authentication succeeded, test failed!", Color::RED);
                return 1;
            }
        }

        println("==>", Color::WHITE);
        // Connect QTcpSocket to server
        socket = new QTcpSocket();
        socket->connectToHost(address, port);
        if (!socket->waitForConnected(WAIT_FOR_READY_READ)) {
            println("Failed to connect to server - timed out, test failed!", Color::RED);
            return 1;
        }
        if (!socket->isValid()) {
            println("Failed to connect to server - invalid socket, test failed!", Color::RED);
            return 1;
        }
        // Try authenticating with correct username and password
        if (socket->isValid()) {
            socket->write(
                    Protocol::createMessage(MESSAGE::AUTHENTICATE, Protocol::getAuthenticationJson("Test", "test123")));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            auto data = socket->readAll();
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation for correct username and password!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (!confirm) {
                println("Authentication failed, test failed!", Color::RED);
                return 1;
            } else {
                println("Authentication succeeded, test succeeded!", Color::BLUE);
            }
        }
        return 0;
    }
}
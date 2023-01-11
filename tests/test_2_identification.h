//
// Created by david on 14. 12. 2022.
//

#ifndef SYSTEMMONITORINGSERVER_TEST_2_IDENTIFICATION_H
#define SYSTEMMONITORINGSERVER_TEST_2_IDENTIFICATION_H

#include <QString>
#include <QTcpSocket>

#include "Utils.h"
#include "protocol.h"

#include "Constants.h"

using namespace Utils;

namespace Test_2 {
    int run(const QString &address, int port) {
        // Connect QTcpSocket to server
        auto socket = new QTcpSocket();
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
        // Try identifying with wrong id
        if (socket->isValid()) {
            auto id = -1;
            socket->write(Protocol::createMessage(MESSAGE::IDENTIFY, Protocol::getIdentificationJson(id)));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            auto data = socket->readAll();
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation for wrong id!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (!confirm) {
                println("Identification failed, test succeeded!", Color::BLUE);
            } else {
                println("Identification succeeded, test failed!", Color::RED);
                return 1;
            }
        }
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
        // Try identifying with correct id
        if (socket->isValid()) {
            auto id = 5; // Test client id
            socket->write(Protocol::createMessage(MESSAGE::IDENTIFY, Protocol::getIdentificationJson(id)));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            auto data = socket->readAll();
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation for correct id!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (!confirm) {
                println("Identification failed, test failed!", Color::RED);
                return 1;
            } else {
                println("Identification succeeded, test succeeded!", Color::BLUE);
            }
        }
        socket->close();
        socket->deleteLater();
        return 0;
    }
}

#endif //SYSTEMMONITORINGSERVER_TEST_2_IDENTIFICATION_H

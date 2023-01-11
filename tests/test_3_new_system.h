//
// Created by pinkynyte on 1/8/23.
//

#ifndef SYSTEMMONITORINGSERVER_TEST_3_NEW_SYSTEM_H
#define SYSTEMMONITORINGSERVER_TEST_3_NEW_SYSTEM_H

#include <QString>
#include <QTcpSocket>

#include "Utils.h"
#include "protocol.h"

#include "Constants.h"

using namespace Utils;

namespace Test_3 {
    int run(const QString &address, int port) {
        auto socket = new QTcpSocket();
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
            socket->write(Protocol::createMessage(MESSAGE::AUTHENTICATE, Protocol::getAuthenticationJson(TEST_USERNAME, TEST_PASSWORD)));
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
            auto id = TEST_CLIENT_ID; // Test client id
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
        // Try sending a new system
        // Create system
        auto system = new System();
        system->addComponent(new Gpu(2670, 24, TEST_CLIENT_ID, 0, "NVIDIA GeForce RTX 4090")); // IMPORTANT: ID = 0, means to add new
//        system->addComponent(new Cpu(4.2, 8, TEST_CLIENT_ID, 0, "Intel Core i7-9700K")); // IMPORTANT: ID = 0, means to add new
//        system->addComponent(new Ram(16, "DDR4", 3200, TEST_CLIENT_ID, 0, "Corsair Vengeance LPX")); // IMPORTANT: ID = 0, means to add new
        // Try sending system
        if (socket->isValid()) {
            socket->write(Protocol::createMessage(MESSAGE::SYSTEM, Protocol::systemToJson(system)));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            auto data = socket->readAll();
            auto messageType = Protocol::getMessageTypes(data)[0];
            if (messageType == MESSAGE::CONFIRM) {
                println("Received confirmation for new system!", Color::BLUE);
            } else {
                println("Received unknown message, test failed!", Color::RED);
                return 1;
            }
            auto messageJson = Protocol::getMessageJsons(data)[0];
            auto confirm = Protocol::getConfirmation(messageJson);
            if (!confirm) {
                println("Sending new system failed, test failed!", Color::RED);
                return 1;
            } else {
                println("Sending new system succeeded, test succeeded!", Color::BLUE);
            }
        }
        socket->close();
        socket->deleteLater();
        return 0;
    }
}

#endif //SYSTEMMONITORINGSERVER_TEST_3_NEW_SYSTEM_H

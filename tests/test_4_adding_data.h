//
// Created by pinkynyte on 1/12/23.
//

#ifndef SYSTEMMONITORINGSERVER_TEST_4_ADDING_DATA_H
#define SYSTEMMONITORINGSERVER_TEST_4_ADDING_DATA_H

#include <QString>
#include <QTcpSocket>

#include "Utils.h"
#include "protocol.h"

using namespace Utils;

namespace Test_4 {
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
            socket->write(Protocol::createMessage(MESSAGE::AUTHENTICATE,
                                                  Protocol::getAuthenticationJson(TEST_USERNAME, TEST_PASSWORD)));
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
        // Identify the cpu
        auto cpuName = CPU::getName();
        auto cpuMaxFrequency = CPU::getMaxFrequency();
        auto cpuCores = CPU::getLogicalCores();
        auto cpuId = 0;
        auto fkClient = TEST_CLIENT_ID;
        auto cpu = new Cpu(cpuMaxFrequency, cpuCores, fkClient, cpuId, cpuName);
        // Add the cpu
        system->addComponent(cpu);
        // Try sending system
        if (socket->isValid()) {
            socket->write(Protocol::createMessage(MESSAGE::SYSTEM, Protocol::systemToJson(system)));
        }
        // Receive data
        if (socket->isValid()) {
            socket->waitForReadyRead(WAIT_FOR_READY_READ);
            // Sleep for 1 second to allow server to process data
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

#endif //SYSTEMMONITORINGSERVER_TEST_4_ADDING_DATA_H

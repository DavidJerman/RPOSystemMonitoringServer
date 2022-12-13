//
// Created by david on 13. 12. 2022.
//

#include "Utils.h"
#include "tests/test_0_connection.h"
#include "tests/test_1_authentication.h"

using namespace Utils;

int run_tests() {
    QString address = "127.0.0.1";
    int port = 1234;
    bool pass = true;

    std::vector<bool> results;

    sleep(1);
    println("=========================================", Color::WHITE);
    println("Running tests...                        |", Color::WHITE);\

    println("=========================================", Color::WHITE);
    println("Running test 0 - Connection             |", Color::WHITE);
    println("-----------------------------------------", Color::WHITE);
    auto res = Test_0::run(address, port);
    if (res != 0) {
        println("Test 0 failed!", Color::RED);
        pass = false;
    } else {
        println("Test 0 passed!", Color::GREEN);
    }
    results.push_back(res == 0);

    println("=========================================", Color::WHITE);
    println("Running test 1 - Authentication         |", Color::WHITE);
    println("-----------------------------------------", Color::WHITE);
    res = Test_1::run(address, port);
    if (res != 0) {
        println("Test 1 failed!", Color::RED);
        pass = false;
    } else {
        println("Test 1 passed!", Color::GREEN);
    }
    results.push_back(res == 0);

    // ...

    // Print results
    println("=========================================", Color::WHITE);
    println("Test results:                           |", Color::WHITE);
    println("-----------------------------------------", Color::WHITE);
    for (int i = 0; i < results.size(); i++) {
        if (results[i]) {
            println("Test " + QString::number(i) + " passed!                          |", Color::GREEN);
        } else {
            println("Test " + QString::number(i) + " failed!                          |", Color::RED);
        }
    }
    println("=========================================", Color::WHITE);

    return pass;
}
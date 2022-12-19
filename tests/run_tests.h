//
// Created by david on 13. 12. 2022.
//

#include "Utils.h"
#include "tests/test_0_connection.h"
#include "tests/test_1_authentication.h"
#include "tests/test_2_identification.h"

using namespace Utils;

int run_tests() {
    QString address = "127.0.0.1";
    int port = 1234;
    bool pass = true;

    std::vector<bool> results;

    println("================================================================", Color::WHITE);
    println("Running tests...                                               |", Color::WHITE);\

    // Test 0
    println("================================================================", Color::WHITE);
    println("Running tests 0 - Connection                                   |", Color::WHITE);
    println("----------------------------------------------------------------", Color::WHITE);
    auto res = Test_0::run(address, port);
    println("----------------------------------------------------------------", Color::WHITE);
    if (res != 0) {
        println("Test 0 failed!                                                 |", Color::RED);
        pass = false;
    } else {
        println("Test 0 passed!                                                 |", Color::GREEN);
    }
    results.push_back(res == 0);

    // Test 1
    println("================================================================", Color::WHITE);
    println("Running tests 1 - Authentication                               |", Color::WHITE);
    println("----------------------------------------------------------------", Color::WHITE);
    res = Test_1::run(address, port);
    println("----------------------------------------------------------------", Color::WHITE);
    if (res != 0) {
        println("Test 1 failed!                                                 |", Color::RED);
        pass = false;
    } else {
        println("Test 1 passed!                                                 |", Color::GREEN);
    }
    results.push_back(res == 0);

    // Test 2
    println("================================================================", Color::WHITE);
    println("Running tests 2 - Identification                               |", Color::WHITE);
    println("----------------------------------------------------------------", Color::WHITE);
    res = Test_2::run(address, port);
    println("----------------------------------------------------------------", Color::WHITE);
    if (res != 0) {
        println("Test 2 failed!                                                 |", Color::RED);
        pass = false;
    } else {
        println("Test 2 passed!                                                 |", Color::GREEN);
    }
    results.push_back(res == 0);

    // Print results
    println("================================================================", Color::WHITE);
    println("Tests results:                                                 |", Color::WHITE);
    println("----------------------------------------------------------------", Color::WHITE);
    for (int i = 0; i < results.size(); i++) {
        if (results[i]) {
            println("Test " + QString::number(i) + " passed!                                                 |", Color::GREEN);
        } else {
            println("Test " + QString::number(i) + " failed!                                                 |", Color::RED);
        }
    }
    println("================================================================", Color::WHITE);

    return pass;
}

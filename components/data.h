//
// Created by david on 19. 12. 2022.
//

#ifndef SYSTEMMONITORINGSERVER_DATA_H
#define SYSTEMMONITORINGSERVER_DATA_H

#ifdef _WIN32
#include <windows.h>
#include <powrprof.h>
#endif // _WIN32


#include "components/cpu.h"
#include "components/gpu.h"
#include "components/ram.h"
#include "components/network.h"
#include "components/disk.h"
#include "components/system.h"

#include <cpuid.h>
#include <iostream>
#include <string>
#include <regex>
#include <fstream>


namespace CPU {
    /**
     * @brief Returns the CPU vendor
     * @return CPU vendor
     */
    static QString getVendor() {
        // Define data container
        int data[4];
        // Obtain the data
        __cpuid(0, data[0], data[1], data[2], data[3]);
        // Create the word
        char vendor[13];
        // Copy the data
        memcpy(vendor, data + 1, 4);
        memcpy(vendor + 4, data + 3, 4);
        memcpy(vendor + 8, data + 2, 4);
        // Add the null terminator
        vendor[12] = '\0';
        // Return the vendor
        return {vendor};
    }

    /**
     * @brief Returns the CPU name
     * @return CPU name
     */
    static QString getName() {
        // Define data container
        int data[4];
        char name[48];
        // Get the data register by register
        __cpuid(0x80000002, data[0], data[1], data[2], data[3]);
        memcpy(name, data, 16);
        __cpuid(0x80000003, data[0], data[1], data[2], data[3]);
        memcpy(name + 16, data, 16);
        __cpuid(0x80000004, data[0], data[1], data[2], data[3]);
        memcpy(name + 32, data, 16);
        // Return
        return {name};
    }

    /**
     * @brief Returns the CPU max frequency
     * @return CPU max frequency
     */
    static double getMaxFrequency() {
#ifdef __linux__
        // Get max cpu frequency
        std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
        // Check if file open
        if (file.is_open()) {
            // Read the file
            std::string line;
            std::getline(file, line);
            // Convert to double
            double maxFrequency = std::stod(line);
            // Return
            return maxFrequency / 1000;
        } else {
            file.close();
            file.open("/proc/cpuinfo");
            if (file.is_open()) {
                std::regex frequencyRegex(R"(cpu MHz\s*:\s*(\d+\.\d+))");
                std::smatch frequencyMatch;
                std::string line;
                while (std::getline(file, line)) {
                    if (std::regex_search(line, frequencyMatch, frequencyRegex)) {
                        // Return the frequency
                        return std::stod(frequencyMatch[1]);
                    }
                }
            }
        }
        return 0;
#endif // __linux__
#ifdef _WIN32
        // Get the structure
        POWER_INFORMATION_LEVEL powerInformationLevel = ProcessorInformation;
        PVOID inputBuffer;
        ULONG inputBufferLength = 0;
        PVOID outputBuffer;
        ULONG outputBufferLength = 0;
        CallNtPowerInformation(powerInformationLevel, inputBuffer, inputBufferLength, outputBuffer, outputBufferLength);
#endif // _WIN32
        return 0.0;
    }

    static int getCores() {

    }

    static int getThreads() {

    }

    /**
     * @brief Returns the CPU frequency
     * @return CPU frequency
     */
    static double getFrequency() {
#ifdef __linux__
        // Open the file
        std::ifstream file("/proc/cpuinfo");
        // Check if the file is open
        if (!file.is_open()) {
            // Return 0
            return 0;
        }
        std::regex frequencyRegex(R"(cpu MHz\s*:\s*(\d+\.\d+))");
        std::smatch frequencyMatch;
        std::string line;
        int c = 0;
        double sum = 0;
        while (std::getline(file, line)) {
            if (std::regex_search(line, frequencyMatch, frequencyRegex)) {
                // Return the frequency
                sum += std::stod(frequencyMatch[1]);
                c++;
            }
        }
        if (c == 0) {
            return 0;
        }
        return sum / c;
#endif // __linux__
#ifdef _WIN32

#endif // _WIN32
        return 0.0;
    }
}

#endif //SYSTEMMONITORINGSERVER_DATA_H

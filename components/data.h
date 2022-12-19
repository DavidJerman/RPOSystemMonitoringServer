//
// Created by david on 19. 12. 2022.
//

#ifndef SYSTEMMONITORINGSERVER_DATA_H
#define SYSTEMMONITORINGSERVER_DATA_H

#ifdef _WIN32
#include <windows.h>
extern "C" {
    #include <Powrprof.h>
}
#pragma comment(lib, "Powrprof.lib")
#endif // _WIN32

#ifdef __linux__
#include <cpuid.h>
#endif // __linux__


#include "components/cpu.h"
#include "components/gpu.h"
#include "components/ram.h"
#include "components/network.h"
#include "components/disk.h"
#include "components/system.h"

#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#ifdef _WIN32
typedef struct PROCESSOR_POWER_INFORMATION {
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;
#endif // _WIN32


namespace CPU {
    // Prototypes
    static QString getVendor();
    static QString getName();
    static double getMaxFrequency();
    static double getFrequency();
    static unsigned long getLogicalCores();
    static bool hasHTT();

    /**
     * @brief Returns the CPU vendor
     * @return CPU vendor
     */
    static QString getVendor() {
        // Define data container
        int data[4];
        // Obtain the data
#ifdef _WIN32
        __cpuid(data, 0);
#endif // _WIN32
#ifdef __linux__
        __cpuid(0, data[0], data[1], data[2], data[3]);
#endif // __linux__
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
#ifdef _WIN32
        __cpuid(data, (int) 0x80000002);
        memcpy(name, data, 16);
        __cpuid(data, (int) 0x80000003);
        memcpy(name + 16, data, 16);
        __cpuid(data, (int) 0x80000004);
        memcpy(name + 32, data, 16);
#endif // _WIN32
#ifdef __linux__
        __cpuid(0x80000002, data[0], data[1], data[2], data[3]);
        memcpy(name, data, 16);
        __cpuid(0x80000003, data[0], data[1], data[2], data[3]);
        memcpy(name + 16, data, 16);
        __cpuid(0x80000004, data[0], data[1], data[2], data[3]);
        memcpy(name + 32, data, 16);
#endif // __linux__
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
        // Get cpu cores
        auto cores = getLogicalCores();
        // Get the structure
        POWER_INFORMATION_LEVEL powerInformationLevel = ProcessorInformation;
        PVOID inputBuffer = new PVOID();
        ULONG inputBufferLength = 0;
        PVOID outputBuffer = new PVOID();
        ULONG outputBufferLength = cores * sizeof(PROCESSOR_POWER_INFORMATION);
        CallNtPowerInformation(powerInformationLevel, inputBuffer, inputBufferLength, outputBuffer, outputBufferLength);
        double sum = 0;
        for (unsigned int i = 0; i < cores; i++) {
            auto processorPowerInformation = (PROCESSOR_POWER_INFORMATION *) outputBuffer;
            sum += processorPowerInformation[i].MaxMhz;
        }
        return sum / cores;
#endif // _WIN32
        return 0.0;
    }

    static unsigned long getPhysicalCores() {
#ifdef _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        // TODO: Improve this, this is just a temporary fix - will not work with CPUs that have a combination of HTT and SMT cores
        return sysInfo.dwNumberOfProcessors / (hasHTT() ? 2 : 1);
#endif // _WIN32
#ifdef __linux__
        // Open the procinfo file
        std::ifstream file("/proc/cpuinfo");
        // Check if file open
        if (file.is_open()) {
            // Read the file
            std::string line;
            std::regex regex(R"(cpu cores\s*:\s*(\d+))");
            std::smatch match;
            while (std::getline(file, line)) {
                if (std::regex_search(line, match, regex)) {
                    // Return the frequency
                    return std::stoul(match[1]);
                }
            }
        }
#endif // __linux__
        return 1;
    }

    /**
     * @brief Returns the number of logical cores
     * @return Number of logical cores
     */
    static unsigned long getLogicalCores() {
        // TODO: Linux implementation
#ifdef _WIN32
        // Get number of cores
        try {
            auto systemInfo = new SYSTEM_INFO();
            GetSystemInfo(systemInfo);
            return (int)systemInfo->dwNumberOfProcessors;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
#endif // _WIN32
#ifdef __linux__
        // Open the procinfo file
        std::ifstream file("/proc/cpuinfo");
        // Check if file open
        if (file.is_open()) {
            // Read the file
            std::string line;
            std::regex regex(R"(processor\s*:\s*(\d+))");
            std::smatch match;
            unsigned long max = 0;
            while (std::getline(file, line)) {
                if (std::regex_search(line, match, regex)) {
                    // Return the frequency
                    auto value = std::stoul(match[1]);
                    if (value > max) {
                        max = value;
                    }
                }
            }
            return max + 1;
        }
#endif // __linux__
        return 1;
    }


    /**
     * @brief Does the cpu have hyper-threading technology?
     * @return True if the cpu has hyper-threading technology
     */
    static bool hasHTT() {
        int data[4];
#ifdef _WIN32
        __cpuid(data, 1);
#endif // _WIN32
#ifdef __linux__
        __cpuid(1, data[0], data[1], data[2], data[3]);
#endif // __linux__
        return data[3] & (1 << 28);
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
        // TODO: Find a better solution - this is not really what I want
        // Get cpu cores
        auto cores = getLogicalCores();
        // Get the structure
        POWER_INFORMATION_LEVEL powerInformationLevel = ProcessorInformation;
        PVOID inputBuffer = new PVOID();
        ULONG inputBufferLength = 0;
        PVOID outputBuffer = new PVOID();
        ULONG outputBufferLength = cores * sizeof(PROCESSOR_POWER_INFORMATION);
        CallNtPowerInformation(powerInformationLevel, inputBuffer, inputBufferLength, outputBuffer, outputBufferLength);
        double sum = 0;
        for (unsigned int i = 0; i < cores; i++) {
            auto processorPowerInformation = (PROCESSOR_POWER_INFORMATION *) outputBuffer;
            sum += processorPowerInformation[i].CurrentMhz;
        }
        return sum / cores;
#endif // _WIN32
        return 0.0;
    }
}

#endif //SYSTEMMONITORINGSERVER_DATA_H

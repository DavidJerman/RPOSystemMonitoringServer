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

#include <windef.h>
#include <Pdh.h>

#pragma comment(lib, "Pdh.lib")

#include <iostream>

#endif // _WIN32

#ifdef __linux__
#include <cpuid.h>
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <unistd.h>
#endif // __linux__


#include "components/cpu.h"
#include "components/gpu.h"
#include "components/ram.h"
#include "components/network.h"
#include "components/disk.h"
#include "components/system.h"


#ifdef _WIN32
typedef struct PROCESSOR_POWER_INFORMATION {
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

struct cpu_counter {
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
};
#endif // _WIN32


namespace CPU {
    // Prototypes
    static QString getVendor();

    static QString getName();

    static double getMaxFrequency();

    static double getFrequency();

    static unsigned long getLogicalCores();

    static bool hasHTT();

#ifdef _WIN32

    int cpu_counter_init(struct cpu_counter *pcc);

    int cpu_counter_get(struct cpu_counter *pcc);

    void cpu_counter_close(struct cpu_counter *pcc);

#endif // _WIN32

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
        auto sysInfo = new SYSTEM_INFO();
        GetSystemInfo(sysInfo);
        // TODO: Improve this, this is just a temporary fix - will not work with CPUs that have a combination of HTT and SMT cores
        return sysInfo->dwNumberOfProcessors / (hasHTT() ? 2 : 1);
        delete sysInfo;
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
     * @return Number of logical cores, if error returns -1
     */
    static unsigned long getLogicalCores() {
#ifdef _WIN32
        // Get number of cores
        try {
            auto systemInfo = new SYSTEM_INFO();
            GetSystemInfo(systemInfo);
            return (int) systemInfo->dwNumberOfProcessors;
        } catch (std::exception &e) {
            return -1;
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

    static double getUtilization() {
#ifdef __linux__
        // Data container
        std::vector<double> data;
        // Open the file
        std::ifstream file("/proc/stat");
        // Check if open
        if (!file.is_open()) {
            return 0;
        }
        // Read the file
        std::string line;
        std::regex regex(R"(cpu\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+))");
        std::smatch match;
        while (std::getline(file, line)) {
            if (std::regex_search(line, match, regex)) {
                // Get the values
                auto user = std::stoul(match[1]);
                auto nice = std::stoul(match[2]);
                auto system = std::stoul(match[3]);
                auto idle = std::stoul(match[4]);
                auto iowait = std::stoul(match[5]);
                auto irq = std::stoul(match[6]);
                auto softirq = std::stoul(match[7]);
                auto steal = std::stoul(match[8]);
                auto guest = std::stoul(match[9]);
                auto guestNice = std::stoul(match[10]);
                // Calculate the total
                auto total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guestNice;
                auto work = user + nice + system + irq + softirq + steal + guest + guestNice;
                data.push_back(total);
                data.push_back(work);
            }
        }
        file.close();
        sleep(1);
        file.open("/proc/stat");
        // Check if open
        if (!file.is_open()) {
            return 0;
        }
        // Read the file
        regex = std::regex(R"(cpu\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+))");
        while (std::getline(file, line)) {
            if (std::regex_search(line, match, regex)) {
                // Get the values
                auto user = std::stoul(match[1]);
                auto nice = std::stoul(match[2]);
                auto system = std::stoul(match[3]);
                auto idle = std::stoul(match[4]);
                auto iowait = std::stoul(match[5]);
                auto irq = std::stoul(match[6]);
                auto softirq = std::stoul(match[7]);
                auto steal = std::stoul(match[8]);
                auto guest = std::stoul(match[9]);
                auto guestNice = std::stoul(match[10]);
                // Calculate the total
                auto total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guestNice;
                auto work = user + nice + system + irq + softirq + steal + guest + guestNice;
                data.push_back(total);
                data.push_back(work);
            }
        }
        file.close();
        // Calculate the utilization
        auto work = data[3] - data[1];
        auto total = data[2] - data[0];
        return work / total * 100;
#endif // __linux__
#ifdef _WIN32
        // https://www.appsloveworld.com/cplus/100/393/c-find-current-cpu-usage-in-hertz-windows
        try {
            cpu_counter counter{};
            cpu_counter_init(&counter);
            Sleep(1000);
            auto res = cpu_counter_get(&counter);
            cpu_counter_close(&counter);
            return res;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            return 0;
        } catch (...) {
            std::cout << "Unknown exception" << std::endl;
            return 0;
        }

#endif // _WIN32
        return 0.0;
    }

#ifdef _WIN32
    // https://www.appsloveworld.com/cplus/100/393/c-find-current-cpu-usage-in-hertz-windows
    /* Initialize query & counter */
    int cpu_counter_init(struct cpu_counter *pcc) {
        if (PdhOpenQueryA(nullptr, 0, &pcc->query) != ERROR_SUCCESS)
            return -1;
        if (PdhAddEnglishCounterA(pcc->query, "\\Processor(_Total)\\% Processor Time", 0, &pcc->counter) !=
            ERROR_SUCCESS || PdhCollectQueryData(pcc->query) != ERROR_SUCCESS) {
            PdhCloseQuery(pcc->query);
            return -2;
        }
        return 0;
    }

/* Fetch data from query and evaluate current counter value */
    int cpu_counter_get(struct cpu_counter *pcc) {
        PDH_FMT_COUNTERVALUE counter_val;
        if (PdhCollectQueryData(pcc->query) != ERROR_SUCCESS ||
            PdhGetFormattedCounterValue(pcc->counter, PDH_FMT_LONG, nullptr, &counter_val) != ERROR_SUCCESS)
            return -1;
        return counter_val.longValue;
    }

/* Close all counters of query and query itself at the end */
    void cpu_counter_close(struct cpu_counter *pcc) {
        if (pcc->query != nullptr) {
            PdhCloseQuery(pcc->query);
            pcc->query = nullptr;
        }
    }

#endif // _WIN32
}


#endif //SYSTEMMONITORINGSERVER_DATA_H

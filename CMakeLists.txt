cmake_minimum_required(VERSION 3.14)

project(SystemMonitoringServer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(SystemMonitoringServer
        main.cpp
        protocol.h protocol.cpp
        config.h config.cpp
        server.h server.cpp
        components/cpu.h components/cpu.cpp
        components/disk.h components/disk.cpp
        components/gpu.h components/gpu.cpp
        components/network.h components/network.cpp
        components/ram.h components/ram.cpp
        components/system.h components/system.cpp
        components/component.h components/component.cpp
        Session.cpp Session.h)
target_link_libraries(SystemMonitoringServer Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(SystemMonitoringServer Qt${QT_VERSION_MAJOR}::Network)


install(TARGETS SystemMonitoringServer
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

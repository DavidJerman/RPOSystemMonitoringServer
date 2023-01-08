cmake_minimum_required(VERSION 3.14)

project(SystemMonitoringServer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_COMPONENTS
        Core
        Network
        Sql
        )

set(SOURCE_FILES
        main.cpp
        protocol.cpp
        config.cpp
        server.cpp
        components/cpu.cpp
        components/disk.cpp
        components/gpu.cpp
        components/network.cpp
        components/ram.cpp
        components/system.cpp
        components/component.cpp
        Session.cpp
        )

set(HEADER_FILES
        protocol.h
        config.h
        server.h
        components/cpu.h
        components/disk.h
        components/gpu.h
        components/network.h
        components/ram.h
        components/system.h
        components/component.h
        Session.h
        Constants.h
        tests/test_1_authentication.h
        tests/test_0_connection.h
        tests/run_tests.h
        Utils.h
        tests/test_2_identification.h
        components/data.h tests/test_3_new_system.h
        )

set(LIBRARIES
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Sql)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_executable(SystemMonitoringServer ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(SystemMonitoringServer PRIVATE ${LIBRARIES})

install(TARGETS SystemMonitoringServer
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

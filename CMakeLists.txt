cmake_minimum_required (VERSION 2.8.10)
PROJECT(DOT)

IF(CMAKE_CXX_COMPILER MATCHES cl.exe)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -DWIN32 -DNOMINMAX)
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11" )
ENDIF()

find_package(Qt5Core 5.3.1 REQUIRED)
find_package(Qt5Gui 5.3.1 REQUIRED)
find_package(Qt5Widgets 5.3.1 REQUIRED)
find_package(Qt5Concurrent 5.3.1 REQUIRED)
find_package(Qt5PrintSupport 5.3.1 REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_subdirectory("Editor+Engine")

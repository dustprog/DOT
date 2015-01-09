cmake_minimum_required (VERSION 2.8.10)
PROJECT(DOT_Alpha)

########################################################################################
# Build settings
########################################################################################

IF(CMAKE_CXX_COMPILER MATCHES cl.exe)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -DWIN32 -DNOMINMAX)
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11" )
ENDIF()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)


########################################################################################
# Prerequisites
########################################################################################
include(ExternalProject)

OPTION(ENABLE_TESTS "Enable unit and functional testing" OFF)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS system)
#find_package(libnabo)
#find_package(Qt5Core)
#find_package(Qt5Gui)
#find_package(Qt5Widgets)
#find_package(Qt5Concurrent)
#find_package(Qt5PrintSupport)
if(ENABLE_TESTS)
    enable_testing()
    ExternalProject_Add(
        GMOCK_BUILD
        DOWNLOAD_DIR ${PROJECT_SOURCE_DIR}/3rd_party/gmock
        URL ""
        UPDATE_COMMAND ""
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/3rd_party/gmock
        INSTALL_DIR ${PROJECT_SOURCE_DIR}/3rd_party/
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    )
    set(GTest_LIBRARIES gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
    set(GMock_LIBRARIES gmock gtest gmock_main ${CMAKE_THREAD_LIBS_INIT})
endif()
# help the cmake find 3rd_party compiled/installed packages
set(CMAKE_PREFIX_PATH  "${CMAKE_PREFIX_PATH};${PROJECT_SOURCE_DIR}/3rd_party")
set(libnabo_DIR "${CMAKE_PREFIX_PATH};${PROJECT_SOURCE_DIR}/3rd_party/libnabo-master")
set(Boost_USE_MULTITHREADED      ON)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(.)
include_directories(3rd_party/include) # all 3rd_party includes built by ExternalProject subsystem are installed there
#include_directories(3rd_party/nabo) # all 3rd_party includes built by ExternalProject subsystem are installed there
link_directories(3rd_party/lib)
set(LINK_WITH ${Boost_LIBRARIES})
add_subdirectory(ZETA_Alpha)
add_subdirectory(DOT_Alpha)
add_subdirectory(TestingGrounds)


cmake_minimum_required(VERSION 3.16)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bins)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(SOURCES_FOLDER "src")
set(TESTS_FOLDER "tests")

project(SimpleAMQP
        VERSION 1.0
        LANGUAGES CXX)

find_library(qpidmessaging 
             NAMES qpidmessaging
             HINTS /usr/local/lib)

if(NOT qpidmessaging)
    message(FATAL_ERROR "libqpidmessaging.so not found")
endif()

file(GLOB ALL_SOURCES "${SOURCES_FOLDER}/*.cpp")
file(GLOB ALL_TESTS "${TESTS_FOLDER}/*.cpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(simpleamqp SHARED ${ALL_SOURCES})
target_link_libraries(simpleamqp qpidmessaging qpidtypes)

add_executable(test ${ALL_TESTS} ${ALL_SOURCES})
target_link_libraries(test simpleamqp qpidmessaging qpidtypes gtest_main)
gtest_discover_tests(test)
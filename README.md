# Simple AMQP Client

This project is a small and simple AMQP client that connects to a QPid broker to send and receive messages.

## Prerequisite

To make and use this project, you will need:
1. Apache Qpid C++ source: https://www.apache.org/dyn/closer.lua?filename=qpid/cpp/1.39.0/qpid-cpp-1.39.0.tar.gz&action=download
2. To build Qpid broker (follow the official instructions): https://raw.githubusercontent.com/apache/qpid-cpp/main/INSTALL.txt
3. To build and install C++ libraries (follow the official instructions): https://raw.githubusercontent.com/apache/qpid-cpp/main/INSTALL.txt
4. CMake 3.16 or newer
5. gcc supporting C++17

## Building Project

To build the project, follow these instructions:
1. git clone "https://github.com/mladja777/SimpleAMQP.git"
2. cd SimpleAMQP/
3. cmake -S . -B build/
4. cmake --build build/ --config Debug

## Library

The built project includes a shared library (libsimpleamqp.so) and a test executable.
Users can include and use this simple client in their projects.
To link the library to their application, users can follow the CMakeLists.txt from the root directory of the project, which is used for linking the library to the test executable.

## Running Tests

Ensure you have a running setup (Qpid broker and queues set up):
1. sudo qpidd --config /etc/qpid/qpidd.conf
2. qpid-config add queue first_sender
3. qpid-config add queue second_sender
4. (Optional: queue some messages) qpid-send --broker localhost:5672 --address first_sender --content-string "Hello, World!"
5. (Be positioned in the root directory of this project)
6. ./bins/test

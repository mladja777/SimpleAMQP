#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include <iostream>

#include "QPIDCommunication.hpp"
#include "TextStorage.hpp"

TEST(StorageTest, TextStorage) {
    SimpleAMQP::TextStorage storage("bins/storage.txt");
    std::unique_ptr<SimpleAMQP::CommunicationInterface> pointer = std::make_unique<SimpleAMQP::QPIDCommunication>();
    EXPECT_TRUE(pointer->Connect("localhost:5672"));
    EXPECT_TRUE(pointer->AddSender("first_sender"));
    EXPECT_TRUE(pointer->AddSender("second_sender"));
    EXPECT_TRUE(pointer->SendMessage("first_sender", "First message"));
    EXPECT_TRUE(pointer->SendMessage("first_sender", "XYZ"));
    EXPECT_TRUE(pointer->SendMessage("second_sender", "QWE"));
    EXPECT_TRUE(pointer->SendMessage("second_sender", "Second message"));
    auto getTime = []() -> std::string {
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::tm bt = *std::localtime(&current_time);
        std::stringstream ss;
        ss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    };
    auto callback = [&](std::string queue, std::string message) -> bool {
        std::cout << "S:Received: " << queue << ", " << message << std::endl;
        storage.SaveData(queue, message, getTime());
        return true;
    };
    EXPECT_TRUE(pointer->AddReceiver("first_sender", callback));
    EXPECT_TRUE(pointer->AddReceiver("second_sender", callback));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}
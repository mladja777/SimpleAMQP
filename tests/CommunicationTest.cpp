#include <gtest/gtest.h>
#include <memory>
#include <iostream>

#include "CommunicationInterface.hpp"
#include "QPIDCommunication.hpp"

class MocCommunication : public SimpleAMQP::CommunicationInterface {
public:
    ~MocCommunication() = default;
    bool AddOption(const std::string& option) override {
        if ("first" == option.substr(0, option.find(':')) ||
            "second" == option.substr(0, option.find(':'))) {
            return true;
        } else {
            return false;
        }
    }
    bool Connect(const std::string& address) override {
        if ("tcp:localhost:1234" == address) {
            return true;
        } else {
            return false;
        }
    }
    bool AddSender(const std::string& queue) {
        return true;
    }
    bool AddReceiver(const std::string& queue, ReceivedMessageHandler&& callback) {
        return true;
    }
    bool SendMessage(const std::string& queue, const std::string& message) {
        return true;
    }
};

TEST(CommunicationTest, MocCommunicationExample) {
    std::unique_ptr<SimpleAMQP::CommunicationInterface> pointer = std::make_unique<MocCommunication>();
    EXPECT_TRUE(pointer->AddOption("first:1"));
    EXPECT_TRUE(pointer->AddOption("second:2"));
    EXPECT_FALSE(pointer->AddOption("third:3"));
    EXPECT_TRUE(pointer->Connect("tcp:localhost:1234"));
    EXPECT_FALSE(pointer->Connect("tcp:localhost:4444"));
}

TEST(CommunicationTest, QPidCommunicationConnection) {
    std::unique_ptr<SimpleAMQP::CommunicationInterface> pointer = std::make_unique<SimpleAMQP::QPIDCommunication>();
    EXPECT_TRUE(pointer->Connect("localhost:5672"));
}

TEST(CommunicationTest, QPidCommunicationSendMessages) {
    std::unique_ptr<SimpleAMQP::CommunicationInterface> pointer = std::make_unique<SimpleAMQP::QPIDCommunication>();
    EXPECT_TRUE(pointer->Connect("localhost:5672"));
    EXPECT_TRUE(pointer->AddSender("first_sender"));
    EXPECT_TRUE(pointer->AddSender("second_sender"));
    EXPECT_FALSE(pointer->AddSender("second_sender"));
    EXPECT_TRUE(pointer->SendMessage("first_sender", "First message"));
    EXPECT_TRUE(pointer->SendMessage("second_sender", "Second message"));
}

TEST(CommunicationTest, QPidCommunicationReceiveMessages) {
    std::unique_ptr<SimpleAMQP::CommunicationInterface> pointer = std::make_unique<SimpleAMQP::QPIDCommunication>();
    EXPECT_TRUE(pointer->Connect("localhost:5672"));
    EXPECT_TRUE(pointer->AddSender("first_sender"));
    EXPECT_TRUE(pointer->AddSender("second_sender"));
    EXPECT_TRUE(pointer->SendMessage("first_sender", "First message"));
    EXPECT_TRUE(pointer->SendMessage("second_sender", "Second message"));
    auto callback = [&](std::string queue, std::string message) -> bool {
        std::cout << "C:Received message form queue '" << queue << "': " << message << std::endl;
        return true;
    };
    EXPECT_TRUE(pointer->AddReceiver("first_sender", callback));
    EXPECT_TRUE(pointer->AddReceiver("second_sender", callback));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}

TEST(CommunicationTest, QPidCommunicationDenyFirst) {
    std::unique_ptr<SimpleAMQP::CommunicationInterface> pointer = std::make_unique<SimpleAMQP::QPIDCommunication>();
    EXPECT_TRUE(pointer->Connect("localhost:5672"));
    EXPECT_TRUE(pointer->AddSender("first_sender"));
    EXPECT_TRUE(pointer->AddSender("second_sender"));
    EXPECT_TRUE(pointer->SendMessage("first_sender", "First message"));
    EXPECT_TRUE(pointer->SendMessage("second_sender", "Second message"));
    EXPECT_TRUE(pointer->AddReceiver("first_sender",
                                     [&](std::string queue, std::string message) -> bool {
        return false;
    }));
    EXPECT_TRUE(pointer->AddReceiver("second_sender",
                                     [&](std::string queue, std::string message) -> bool {
        return true;
    }));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}
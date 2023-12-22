#include "QPIDCommunication.hpp"

#include <qpid/messaging/Message.h>
#include <chrono>

namespace SimpleAMQP {

QPIDCommunication::QPIDCommunication() {
    mReceiverThread.reset(new std::thread([&](){
        while (mShouldReceive) {
            using namespace std::chrono_literals;
            using namespace qpid::messaging;
            std::this_thread::sleep_for(100ms);
            std::unique_lock<std::mutex> lock(mObjectMutex);
            for (auto& [queue, receiverCbPair] : mReceiverMap) {
                try {
                    auto& [receiver, callback] = receiverCbPair;
                    while (true) {
                        auto response = receiver.fetch(Duration::IMMEDIATE);
                        auto valid = callback(queue, response.getContentObject());
                        if (valid) {
                            mSession.acknowledge(response);
                            continue;
                        }
                        if (mSenderMap.find(queue) == mSenderMap.end()) {
                            mSenderMap.emplace(queue, mSession.createSender(queue));
                        }
                        Message message;
                        message.setReplyTo(queue);
                        std::string rsp("Message not valid: " + response.getContentObject());
                        message.setContentObject(rsp);
                        mSenderMap.at(queue).send(rsp);
                    }
                } catch (...) {}
            }
        }
    }));
}

QPIDCommunication::~QPIDCommunication() {
    mShouldReceive = false;
    if (nullptr != mConnection) {
        mConnection->close();
    }
}

bool QPIDCommunication::AddOption(const std::string& option) {
    const auto& [_, result] = mOptions.insert(option);
    return result;
}

bool QPIDCommunication::Connect(const std::string& address) {
    if (nullptr != mConnection) {
        return false;
    }
    try {
        using namespace qpid::messaging;
        mConnection = std::make_unique<Connection>(address, GetOptionsString());
        mConnection->open();
        mSession = mConnection->createSession();
    } catch (...) {
        return false;
    }
    return true;
}

std::string QPIDCommunication::GetOptionsString() const {
    std::string options;
    if (!mOptions.empty()) {
        options += "{";
        for (const auto& option : mOptions) {
            options += option;
            options += ",";
        }
        if (options.size() > 1) {
            options[options.size() - 1] = '}';
        } else {
            options += "}";
        }
    }
    return options;
}

bool QPIDCommunication::AddSender(const std::string& queue) {
    std::unique_lock<std::mutex> lock(mObjectMutex);
    if (mSenderMap.find(queue) != mSenderMap.end()) {
        return false;
    }
    const auto& [_, result] = mSenderMap.emplace(queue, mSession.createSender(queue));
    return result;
}

bool QPIDCommunication::AddReceiver(const std::string& queue,
                                    QPIDCommunication::ReceivedMessageHandler&& callback) {
    std::unique_lock<std::mutex> lock(mObjectMutex);
    if (mReceiverMap.find(queue) != mReceiverMap.end()) {
        return false;
    }
    const auto& [_, result] = mReceiverMap.emplace(
                                std::piecewise_construct,
                                std::forward_as_tuple(queue),
                                std::forward_as_tuple(mSession.createReceiver(queue), std::move(callback)));
    return result;
}

bool QPIDCommunication::SendMessage(const std::string& queue, const std::string& message) {
    std::unique_lock<std::mutex> lock(mObjectMutex);
    try {
        mSenderMap.at(queue).send(message);
    } catch (...) {
        return false;
    }
    return true;
}

} // namespace SimpleAMQP
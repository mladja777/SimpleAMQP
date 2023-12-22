#ifndef SIMPLEAMQP_INCLUDE_QPIDCOMMUNICATION_HPP
#define SIMPLEAMQP_INCLUDE_QPIDCOMMUNICATION_HPP

#include "CommunicationInterface.hpp"
#include "Common.hpp"

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Session.h>
#include <memory>
#include <set>
#include <map>
#include <thread>
#include <mutex>

namespace SimpleAMQP {

class DLLExport QPIDCommunication : public CommunicationInterface {
public:
    /**
     * @brief Construct a new QPIDCommunication object
     */
    QPIDCommunication();
    /**
     * @brief Destroy the Communicatione object
     */
    ~QPIDCommunication() override;
    /**
     * @brief Adds an option to be provided for the connection
     * 
     * @param option represents an option for the client in the string
     * format "key:value"
     * @return true if option successfully added
     * @return false if option is not supported
     */
    bool AddOption(const std::string& option) override;
    /**
     * @brief Connects to the AMQP broker with a given address
     * 
     * @param address represents an address of an AMQP broker
     * @return true if connection is successful
     * @return false if connection fails
     */
    bool Connect(const std::string& address) override;
    /**
     * @brief Add sender for a specific queue
     * 
     * @param queue represents a queue name
     * @return true if adding sender was successful
     * @return false if adding sender failed
     */
    bool AddSender(const std::string& queue) override;
    /**
     * @brief Adds a new receiver for a certain queue
     * 
     * @param queue represents a queue name
     * @param callback represents a callback that will be invoked when a message arrives
     * @return true if receiver was added
     * @return false false if receiver adding failed
     */
    bool AddReceiver(const std::string& queue, ReceivedMessageHandler&& callback) override;
    /**
     * @brief Sends a message to the specific queue
     * 
     * @param queue represents a queue name
     * @param message represents a message
     * @return true if sending was successful
     * @return false if sending was not successful
     */
    bool SendMessage(const std::string& queue, const std::string& message) override;

private:
    std::string GetOptionsString() const;

    std::unique_ptr<qpid::messaging::Connection> mConnection = nullptr;
    qpid::messaging::Session mSession;
    std::set<std::string> mOptions;
    std::mutex mObjectMutex;

    using SenderMap = std::map<std::string, qpid::messaging::Sender>;
    SenderMap mSenderMap;

    using ReceiverCallbackPair = std::pair<qpid::messaging::Receiver, ReceivedMessageHandler>;
    using ReceiverMap = std::map<std::string, ReceiverCallbackPair>;
    ReceiverMap mReceiverMap;
    bool mShouldReceive = true;
    struct Deleter {
        void operator()(std::thread* t) const {
            if (nullptr == t) {
                return;
            }
            t->join();
            delete t;
        }
    };
    using ThreadPtr = std::unique_ptr<std::thread, Deleter>;
    ThreadPtr mReceiverThread = nullptr;
};

} // namespace SimpleAMQP


#endif // SIMPLEAMQP_INCLUDE_QPIDCOMMUNICATION_HPP
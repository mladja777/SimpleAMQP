#ifndef SIMPLEAMQP_INCLUDE_COMMUNICATIONINTERFACE_HPP
#define SIMPLEAMQP_INCLUDE_COMMUNICATIONINTERFACE_HPP

#include <string>
#include <functional>

namespace SimpleAMQP {

/**
 * @brief Interface class used to define communications API
 */
class CommunicationInterface {
public:
    using ReceivedMessageHandler = std::function<bool(std::string, std::string)>;
    /**
     * @brief Destroy the Communication Interface object
     */
    virtual ~CommunicationInterface() = default;
    /**
     * @brief Adds an option to be provided for the connection
     * 
     * @param option represents an option for the client in the string
     * format "key:value"
     * @return true if option successfully added
     * @return false if option is not supported
     */
    virtual bool AddOption(const std::string& option) = 0;
    /**
     * @brief Connects to the AMQP broker with a given address
     * 
     * @param address represents an address of an AMQP broker
     * @return true if connection is successful
     * @return false if connection fails
     */
    virtual bool Connect(const std::string& address) = 0;
    /**
     * @brief Add sender for a specific queue
     * 
     * @param queue represents a queue name
     * @return true if adding sender was successful
     * @return false if adding sender failed
     */
    virtual bool AddSender(const std::string& queue) = 0;
    /**
     * @brief Adds a new receiver for a certain queue
     * 
     * @param queue represents a queue name
     * @param callback represents a callback that will be invoked when a message arrives
     * @return true if receiver was added
     * @return false false if receiver adding failed
     */
    virtual bool AddReceiver(const std::string& queue, ReceivedMessageHandler&& callback) = 0;
    /**
     * @brief Sends a message to the specific queue
     * 
     * @param queue represents a queue name
     * @param message represents a message
     * @return true if sending was successful
     * @return false if sending was not successful
     */
    virtual bool SendMessage(const std::string& queue, const std::string& message) = 0;
};

} // namespace SimpleAMQP

#endif // SIMPLEAMQP_INCLUDE_COMMUNICATIONINTERFACE_HPP
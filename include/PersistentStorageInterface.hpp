#ifndef SIMPLEAMQP_INCLUDE_PERSISTENTSTORAGEINTERFACE_HPP
#define SIMPLEAMQP_INCLUDE_PERSISTENTSTORAGEINTERFACE_HPP

#include <string>
#include <tuple>
#include <vector>

namespace SimpleAMQP {

/**
 * @brief Interface class used to define storage API
 */
class PersistentStorageInterface {
public:
    using DataTuple = std::tuple<std::string, std::string, std::string>;
    using DataVector = std::vector<DataTuple>;
    /**
     * @brief Destroy the Persistent Storage Interface object
     */
    virtual ~PersistentStorageInterface() = default;
    /**
     * @brief Save data to storage
     * 
     * @param queue represents queue name
     * @param data represents data to be saved
     * @param timestamp represents a timestamp when it has been
     * processed
     * @return true if successful
     * @return false if not successful
     */
    virtual bool SaveData(const std::string& queue,
                          const std::string& data,
                          const std::string& timestamp) = 0;
    /**
     * @brief Get the Data object returns the data vector
     * 
     * @return DataVector represents an array of data tuples 
     */
    virtual const DataVector& GetData() = 0;
    /**
     * @brief Clears data from database
     */
    virtual void ClearData() = 0;
};

} // namespace SimpleAMQP

#endif // SIMPLEAMQP_INCLUDE_PERSISTENTSTORAGEINTERFACE_HPP
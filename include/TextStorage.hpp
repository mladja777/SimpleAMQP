#ifndef SIMPLEAMQP_INCLUDE_TEXTSTORAGE_HPP
#define SIMPLEAMQP_INCLUDE_TEXTSTORAGE_HPP

#include "Common.hpp"
#include "PersistentStorageInterface.hpp"

namespace SimpleAMQP {

/**
 * @brief Text storage represents a text file that holds data 
 */
class DLLExport TextStorage : public PersistentStorageInterface {
public:
    TextStorage(std::string path);
    /**
     * @brief Destroy the Persistent Storage Interface object
     */
    ~TextStorage() override;
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
    bool SaveData(const std::string& queue,
                  const std::string& data,
                  const std::string& timestamp) override;
    /**
     * @brief Get the Data object returns the data vector
     * 
     * @return DataVector represents an array of data tuples 
     */
    const DataVector& GetData() override;
    /**
     * @brief Clears data from database
     */
    void ClearData() override;

private:
    std::string mPath;
    PersistentStorageInterface::DataVector mData;
};

} // namespace SimpleAMQP

#endif // SIMPLEAMQP_INCLUDE_TEXTSTORAGE_HPP
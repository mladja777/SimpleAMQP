#include "TextStorage.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

namespace SimpleAMQP {

TextStorage::TextStorage(std::string path) : mPath(std::move(path)) {
    std::ifstream storage(mPath);
    std::string line;
    while (std::getline(storage, line)) {
        std::stringstream ss(line);
        std::string part1, part2, part3;

        if (std::getline(ss, part1, '|') &&
            std::getline(ss, part2, '|') &&
            std::getline(ss, part3)) {
            mData.emplace_back(part1, part2, part3);
        }
    }
    storage.close();
}

TextStorage::~TextStorage() {
    std::ofstream storage(mPath, std::ios::out | std::ios::trunc);
    if (!storage.is_open()) {
        return;
    }

    for (const auto& tuple : mData) {
        storage << std::get<0>(tuple) << "|"
                << std::get<1>(tuple) << "|"
                << std::get<2>(tuple) << std::endl;
    }
    storage.close();
}

bool TextStorage::SaveData(const std::string& queue,
                           const std::string& data,
                           const std::string& timestamp) {
    mData.emplace_back(queue, data, timestamp);
    return true;
}
                
const PersistentStorageInterface::DataVector& TextStorage::GetData() {
    return mData;
}

void TextStorage::ClearData() {
    mData.clear();
}

} // namespace SimpleAMQP

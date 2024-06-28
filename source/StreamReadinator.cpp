#include "StreamReadinator.hpp"

namespace JSONJay {

bool StreamReadinator::readUntil(std::vector<char>& data, char delim) {
    char c;
    while (readData(&c, 1)) {
        data.push_back(c);
        if (c == delim) {
            return true;
        }
    }
    return false;
}

bool StreamReadinator::readUntil(std::vector<char>& data, std::string delim) {
    std::vector<char> buffer(delim.size());
    while (readData(buffer.data(), buffer.size())) {
        data.insert(data.end(), buffer.begin(), buffer.end());
        if (data.size() >= delim.size()) {
            if (std::equal(delim.begin(), delim.end(), data.end() - delim.size())) {
                return true;
            }
        }
    }
    return false;
}

bool StreamReadinator::readBuffer(std::vector<char>& buffer, uint32_t size) {
    uint32_t bufferSize = size;
    if (bufferSize == 0) {
        readData(reinterpret_cast<char*>(&bufferSize), sizeof(bufferSize));
    }
    buffer.resize(bufferSize);
    return readData(buffer.data(), buffer.size());
}

bool StreamReadinator::readString(std::string& str) {
    size_t size;
    if(!readData(reinterpret_cast<char*>(&size), sizeof(size))) 
        return false;

    str.resize(size);
    return readData(str.data(), str.size());
}

} // namespace JSONJay
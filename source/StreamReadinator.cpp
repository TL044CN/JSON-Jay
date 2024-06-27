#include "StreamReadinator.hpp"

namespace JSONJay {

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
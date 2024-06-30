#include "StreamReadinator.hpp"

#include <deque>

namespace JSONJay {

bool StreamReadinator::readUntil(std::vector<char>& data, char delim, bool includeDelim) {
    char c;
    while (readData(&c, 1)) {
        data.push_back(c);
        if(c != delim) continue;

        if(includeDelim) return true;

        data.pop_back();
        setStreamPosition(getStreamPosition() - 1);
        return true;
    }
    return false;
}

bool StreamReadinator::readUntil(std::vector<char>& data, const std::string& delim, bool includeDelim) {
    std::vector<char> buffer;
    if(!readBuffer(data, delim.size())){
        return false;
    }

    char c;
    while(readData(&c, 1)){
        data.push_back(c);
        if(std::equal(data.end() - delim.size(), data.end(), delim.begin(), delim.end())){
            if(includeDelim) return true;            

            data.erase(data.end() - delim.size(), data.end());
            setStreamPosition(getStreamPosition() - delim.size());
            return true;
        }
    }
    return false;
}

bool StreamReadinator::readUntil(std::string& data, char delim, bool includeDelim) {
    std::vector<char> buffer;
    if (readUntil(buffer, delim, includeDelim)) {
        data = std::string(buffer.begin(), buffer.end());
        return true;
    }
    return false;
}

bool StreamReadinator::readUntil(std::string& data, const std::string& delim, bool includeDelim) {
    std::vector<char> buffer;
    if (readUntil(buffer, delim, includeDelim)) {
        data = std::string(buffer.begin(), buffer.end());
        return true;
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
#include "StreamWritinator.hpp"

namespace JSONJay {

void StreamWritinator::writeBuffer(const std::vector<char>& buffer, bool writeSize) {
    if (writeSize) {
        uint64_t bufferSize = buffer.size();
        writeData(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));
    }
    writeData(buffer.data(), buffer.size());
}

void StreamWritinator::writeZero(uint64_t size) {
    std::vector<char> zero(size, 0);
    writeData(zero.data(), zero.size());
}

void StreamWritinator::writeString(const std::string& str, bool writeSize) {
    writeBuffer(std::vector<char>(str.begin(), str.end()), writeSize);
}

void StreamWritinator::writeString(std::string_view str, bool writeSize) {
    writeBuffer(std::vector<char>(str.begin(), str.end()), writeSize);
}

} // namespace JSONJay
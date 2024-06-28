#include "StreamWritinator.hpp"

namespace JSONJay {

void StreamWritinator::writeBuffer(const std::vector<char>& buffer, bool writeSize) {
    if (writeSize) {
        size_t bufferSize = buffer.size();
        writeData(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));
    }
    writeData(buffer.data(), buffer.size());
}

void StreamWritinator::writeZero(uint64_t size) {
    std::vector<char> zero(size, 0);
    writeData(zero.data(), zero.size());
}

void StreamWritinator::writeString(const std::string& str) {
    writeBuffer(std::vector<char>(str.begin(), str.end()), true);
}

void StreamWritinator::writeString(std::string_view str) {
    writeBuffer(std::vector<char>(str.begin(), str.end()), true);
}

} // namespace JSONJay
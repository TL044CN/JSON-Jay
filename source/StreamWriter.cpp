#include "StreamWriter.hpp"

namespace JSONJay {

void StreamWriter::writeBuffer(const std::vector<char>& buffer, bool writeSize) {
    if (writeSize) {
        size_t bufferSize = buffer.size();
        writeData(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));
    }
    writeData(buffer.data(), buffer.size());
}

void StreamWriter::writeZero(uint64_t size) {
    std::vector<char> zero(size, 0);
    writeData(zero.data(), zero.size());
}

void StreamWriter::writeString(const std::string& str) {
    writeBuffer(std::vector<char>(str.begin(), str.end()), true);
}

void StreamWriter::writeString(std::string_view str) {
    writeBuffer(std::vector<char>(str.begin(), str.end()), true);
}

} // namespace JSONJay
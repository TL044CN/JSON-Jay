#include "FileStreamReadinator.hpp"

namespace JSONJay {

FileStreamReadinator::FileStreamReadinator(const std::string& filename) {
    mFile.open(filename, std::ios::binary);
}

FileStreamReadinator::~FileStreamReadinator() {
    mFile.close();
}


bool FileStreamReadinator::isStreamGood() const {
    return mFile.good();
}

uint64_t FileStreamReadinator::getStreamPosition() {

    return mFile.tellg();
}

void FileStreamReadinator::setStreamPosition(uint64_t position) {
    mFile.seekg(position);
}

bool FileStreamReadinator::readData(char* data, uint64_t size) {
    mFile.read(data, size);
    return true;
}

} // namespace JSONJay

#include "FileStreamWritinator.hpp"

namespace JSONJay {

FileStreamWritinator::FileStreamWritinator(const std::string& filename) {
    mFile.open(filename, std::ios::binary);
}

FileStreamWritinator::~FileStreamWritinator() {
    mFile.close();
}


bool FileStreamWritinator::isStreamGood() const {
    return mFile.good();
}

uint64_t FileStreamWritinator::getStreamPosition() {
    return mFile.tellp();
}

void FileStreamWritinator::setStreamPosition(uint64_t position) {
    mFile.seekp(position);
}

bool FileStreamWritinator::writeData(const char* data, uint64_t size) {
    mFile.write(data, size);
    return true;
}

} // namespace JSONJay
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

uint64_t FileStreamReadinator::getStreamPosition() const {

    auto state = mFile.rdstate();
    if(state & std::ios_base::badbit)
        throw std::ios_base::failure("Bad bit set");

    if(state & std::ios_base::failbit)
        throw std::ios_base::failure("Fail bit set");

    // this would be undefined behavior if the failbit is set.
    // since we check that beforehand, this is fine.
    auto pos = const_cast<std::ifstream*>(&mFile)->tellg();
    return pos;
}

void FileStreamReadinator::setStreamPosition(uint64_t position) {
    mFile.seekg(position);
}

bool FileStreamReadinator::readData(char* data, uint64_t size) {
    mFile.read(data, size);
    return mFile.good();
}

} // namespace JSONJay

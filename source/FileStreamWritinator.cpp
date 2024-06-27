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

uint64_t FileStreamWritinator::getStreamPosition() const {

    auto state = mFile.rdstate();
    if(state & std::ios_base::badbit)
        throw std::ios_base::failure("Bad bit set");

    if(state & std::ios_base::failbit)
        throw std::ios_base::failure("Fail bit set");

    // this would be undefined behavior if the failbit is set.
    // since we check that beforehand, this is fine.
    auto pos = const_cast<std::ofstream*>(&mFile)->tellp();
    return pos;
}

void FileStreamWritinator::setStreamPosition(uint64_t position) {
    mFile.seekp(position);
}

bool FileStreamWritinator::writeData(const char* data, uint64_t size) {
    mFile.write(data, size);
    return mFile.good();
}

} // namespace JSONJay
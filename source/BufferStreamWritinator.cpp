#include "BufferStreamWritinator.hpp"

namespace JSONJay {

BufferStreamWritinator::BufferStreamWritinator() : position(0) {}

BufferStreamWritinator::~BufferStreamWritinator() {}


bool BufferStreamWritinator::isStreamGood() const {
    return true;
}

uint64_t BufferStreamWritinator::getStreamPosition() {
    return position;
}

void BufferStreamWritinator::setStreamPosition(uint64_t position) {
    this->position = position;
}

bool BufferStreamWritinator::writeData(const char* data, uint64_t size) {
    for (uint64_t i = 0; i < size; i++) {
        buffer.push_back(data[i]);
    }

    position += size;
    return true;
}

const std::vector<char>& BufferStreamWritinator::getBuffer() const {
    return buffer;
}

} // namespace JSONJay
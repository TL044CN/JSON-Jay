#include "BufferStreamReadinator.hpp"

namespace JSONJay {

BufferStreamReadinator::BufferStreamReadinator() : position(0) {}
BufferStreamReadinator::BufferStreamReadinator(const std::vector<char>& buffer) : buffer(buffer), position(0) {}

BufferStreamReadinator::~BufferStreamReadinator() {}


bool BufferStreamReadinator::isStreamGood() const {
    return position < buffer.size();
}

uint64_t BufferStreamReadinator::getStreamPosition() {
    return position;
}

void BufferStreamReadinator::setStreamPosition(uint64_t position) {
    this->position = position;
}

bool BufferStreamReadinator::readData(char* data, uint64_t size) {
    if (position + size > buffer.size()) {
        return false;
    }

    for (uint64_t i = 0; i < size; i++) {
        data[i] = buffer[position + i];
    }

    position += size;
    return true;
}

} // namespace JSONJay
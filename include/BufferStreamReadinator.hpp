/**
 * @file BufferStreamReadinator.hpp
 * @author TL044CN
 * @brief BufferStreamReadinator class declaration
 * @version 0.1
 * @date 2024-06-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "StreamReadinator.hpp"
#include <vector>

namespace JSONJay {

/**
 * @ingroup Serialization
 * @brief BufferStreamReadinator class
 * @details The BufferStreamReadinator class is a StreamReadinator that reads
 *          data fropm a buffer. Buffers are std::vector<char> objects.
 */
class BufferStreamReadinator : public StreamReadinator {
private:
    std::vector<char> buffer;
    uint64_t position;

public:
    /**
     * @brief Construct a new BufferStreamReadinator object
     */
    BufferStreamReadinator();

    /**
     * @brief Construct a new BufferStreamReadinator object
     * 
     * @param buffer the buffer to read from
     */
    BufferStreamReadinator(const std::vector<char>& buffer);

    /**
     * @brief Destroy the BufferStreamReadinator object
     */
    ~BufferStreamReadinator() override;

    /**
     * @brief checks if the stream is good
     *
     * @return true stream is good
     * @return false stream is bad
     */
    bool isStreamGood() const override;

    /**
     * @brief returns the current stream position
     *
     * @return uint64_t the current stream position
     */
    uint64_t getStreamPosition() override;

    /**
     * @brief sets the stream position
     *
     * @param position the position to set
     */
    void setStreamPosition(uint64_t position) override;

    /**
     * @brief reads data from the stream
     *
     * @param data the buffer to read into
     * @param size the size of the buffer
     * @return true read was successful
     * @return false read was unsuccessful
     */
    bool readData(char* data, uint64_t size) override;

};

} // namespace JSONJay
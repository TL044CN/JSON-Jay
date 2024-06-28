/**
 * @file BufferStreamWritinator.hpp
 * @author TL044CN
 * @brief BufferStreamWritinator class declaration
 * @version 0.1
 * @date 2024-06-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "StreamWritinator.hpp"
#include <vector>

namespace JSONJay {

/**
 * @ingroup Serialization
 * @brief BufferStreamWritinator class
 * @details The BufferStreamWritinator class is a StreamWritinator that writes
 *          data to a buffer. Buffers are std::vector<char> objects.
 */
class BufferStreamWritinator : public StreamWritinator {
private:
    std::vector<char> buffer;
    uint64_t position;

public:
    /**
     * @brief Construct a new BufferStreamWritinator object
     */
    BufferStreamWritinator();

    /**
     * @brief Destroy the BufferStreamWritinator object
     */
    ~BufferStreamWritinator() override;

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
     * @brief writes data to the stream
     *
     * @param data the data to write
     * @param size the size of the data
     */
    bool writeData(const char* data, uint64_t size) override;

    /**
     * @brief returns the buffer
     *
     * @return const std::vector<char>& the buffer
     */
    const std::vector<char>& getBuffer() const;

};

} // namespace JSONJay
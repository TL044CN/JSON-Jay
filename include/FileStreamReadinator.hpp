/**
 * @file FileStreamReadinator.hpp
 * @author TL044CN
 * @brief This file contains the FileStreamReadinator class declaration
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "StreamReadinator.hpp"
#include <fstream>

namespace JSONJay {

class FileStreamReadinator : public StreamReadinator {
private:
    std::ifstream mFile;

public:
    explicit FileStreamReadinator(const std::string& filename);
    ~FileStreamReadinator() override;

    bool isStreamGood() const override;
    uint64_t getStreamPosition() const override;
    void setStreamPosition(uint64_t position) override;
    bool readData(char* data, uint64_t size) override;

};

} // namespace JSONJay
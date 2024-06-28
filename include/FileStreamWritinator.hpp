/**
 * @file FileStreamWritinator.hpp
 * @author TL044CN
 * @brief This file contains the FileStreamWritinator class declaration
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "StreamWritinator.hpp"
#include <fstream>

namespace JSONJay {

/**
 * @ingroup Serialization
 * @brief The FileStreamWritinator class is a class that writes data to a file
 * @details The FileStreamWritinator class is a class that writes binary data to a file
 */
class FileStreamWritinator : public StreamWritinator {
private:
    std::ofstream mFile;

public:
    explicit FileStreamWritinator(const std::string& filename);
    ~FileStreamWritinator() override;

    bool isStreamGood() const override;
    uint64_t getStreamPosition() override;
    void setStreamPosition(uint64_t position) override;
    bool writeData(const char* data, uint64_t size) override;
};

} // namespace JSONJay

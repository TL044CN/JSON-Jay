/**
 * @file Exceptions.hpp
 * @author TL044CN
 * @brief Exceptions for the JSONJay library
 * @version 0.1
 * @date 2024-04-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <exception>
#include <string>

namespace JSONJay {

/**
 * @brief Base exception class for the JSONJay library
 */
class JSONJayException : public std::exception {
private:
    std::string mMessage;
public:
    explicit JSONJayException(const std::string& message = "Unknown Exception"): mMessage(message) {}

    virtual const char* what() const noexcept override {
        return ("JSONJay Exception: " + mMessage).c_str();
    }
};

/**
 * @brief Exception thrown when an invalid format is detected
 */
class InvalidFormatException : public JSONJayException {
public:
    explicit InvalidFormatException(const std::string& message = "Invalid Format"): JSONJayException(message) {}
};

/**
 * @brief Exception thrown when an invalid type is detected
 */
class InvalidTypeException : public JSONJayException {
public:
    explicit InvalidTypeException(const std::string& message = "Invalid Type"): JSONJayException(message) {}
};

/**
 * @brief Exception thrown when an invalid key is detected
 */
class InvalidKeyException : public JSONJayException {
public:
    explicit InvalidKeyException(const std::string& message = "Invalid Key"): JSONJayException(message) {}
};

/**
 * @brief Exception thrown when an invalid value is detected
 */
class InvalidValueException : public JSONJayException {
public:
    explicit InvalidValueException(const std::string& message = "Invalid Value"): JSONJayException(message) {}
};

/**
 * @brief Exception thrown when an invalid index is detected
 */
class InvalidIndexException : public JSONJayException {
public:
    explicit InvalidIndexException(const std::string& message = "Invalid Index"): JSONJayException(message) {}
};

/**
 * @brief Exception thrown when a function is called that is not or cannot be implemented
 */
class MissingImplementationException : public JSONJayException {
public:
    explicit MissingImplementationException(const std::string& message = "Missing Implementation"): JSONJayException(message) {}
};

} // namespace JSONJay

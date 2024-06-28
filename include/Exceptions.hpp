/**
 * @file Exceptions.hpp
 * @author TL044CN
 * @brief Exceptions for the JSONJay library
 * @version 0.2
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
 * @ingroup Exceptions
 * @brief Base exception class for the JSONJay library
 */
class JSONJayException : public std::exception {
private:
    std::string mMessage;
public:
    /**
     * @brief Construct a new JSONJayException object
     * 
     * @param message The message to be displayed
     */
    explicit JSONJayException(const std::string& message = "Unknown Exception"): mMessage("JSONJay Exception: " + message) {}

    /**
     * @brief Get the message
     * 
     * @return const char* The message
     */
    const char* what() const noexcept override {
        return mMessage.c_str();
    }
};

/**
 * @ingroup Exceptions
 * @brief Exception thrown when an invalid format is detected
 */
class InvalidFormatException : public JSONJayException {
public:
    /**
     * @brief Construct a new Invalid Format Exception object
     * 
     * @param message The message to be displayed
     */
    explicit InvalidFormatException(const std::string& message = "Invalid Format"): JSONJayException(message) {}
};

/**
 * @ingroup Exceptions
 * @brief Exception thrown when an invalid type is detected
 */
class InvalidTypeException : public JSONJayException {
public:
    /**
     * @brief Construct a new Invalid Type Exception object
     * 
     * @param message The message to be displayed
     */
    explicit InvalidTypeException(const std::string& message = "Invalid Type"): JSONJayException(message) {}
};

/**
 * @ingroup Exceptions
 * @brief Exception thrown when an invalid key is detected
 */
class InvalidKeyException : public JSONJayException {
public:
    /**
     * @brief Construct a new Invalid Key Exception object
     * 
     * @param message The message to be displayed
     */
    explicit InvalidKeyException(const std::string& message = "Invalid Key"): JSONJayException(message) {}
};

/**
 * @ingroup Exceptions
 * @brief Exception thrown when an invalid value is detected
 */
class InvalidValueException : public JSONJayException {
public:
    /**
     * @brief Construct a new Invalid Value Exception object
     * 
     * @param message The message to be displayed
     */
    explicit InvalidValueException(const std::string& message = "Invalid Value"): JSONJayException(message) {}
};

/**
 * @ingroup Exceptions
 * @brief Exception thrown when an invalid index is detected
 */
class InvalidIndexException : public JSONJayException {
public:
    /**
     * @brief Construct a new Invalid Index Exception object
     * 
     * @param message The message to be displayed
     */
    explicit InvalidIndexException(const std::string& message = "Invalid Index"): JSONJayException(message) {}
};

/**
 * @ingroup Exceptions
 * @brief Exception thrown when a function is called that is not or cannot be implemented
 */
class MissingImplementationException : public JSONJayException {
public:
    /**
     * @brief Construct a new Missing Implementation Exception object
     * 
     * @param message The message to be displayed
     */
    explicit MissingImplementationException(const std::string& message = "Missing Implementation"): JSONJayException(message) {}
};

} // namespace JSONJay

/**
 * @file Serializer.hpp
 * @author TL044CN
 * @brief Serializer base class
 * @version 0.1
 * @date 2024-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "Exceptions.hpp"
#include "Common.hpp"
#include "List.hpp"
#include "Object.hpp"

namespace JSONJay {

    /**
     * @brief Serializer base class
     */
    class Serializer {
    public:
        Serializer() = default;
        virtual ~Serializer() = default;

    protected:

        /**
         * @brief Serialize a string
         * 
         * @param data the string to serialize
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_string(const std::string& data, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_string(const std::string&, std::vector<char>&)");        
        };

        /**
         * @brief Serialize a number
         * 
         * @param number the number to serialize
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_number(uint8_t number, std::vector<char>& buffer) {
            throw JSONJay::MissingImplementationException("serialize_number(uint8_t, std::vector<char>&)");
        }
        
        virtual void serialize_number(uint16_t number, std::vector<char>& buffer) {
            throw JSONJay::MissingImplementationException("serialize_number(uint16_t, std::vector<char>&)");        
        };
        virtual void serialize_number(uint32_t number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(uint32_t, std::vector<char>&)");
        }
        virtual void serialize_number(uint64_t number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(uint64_t, std::vector<char>&)");
        }
        virtual void serialize_number(int8_t number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(int_8, std::vector<char>&)");
        }
        virtual void serialize_number(int16_t number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(int16_t, std::vector<char>&)");
        }
        virtual void serialize_number(int32_t number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(int32_t, std::vector<char>&)");
        }
        virtual void serialize_number(int64_t number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(int64_t, std::vector<char>&)");
        }

        /**
         * @brief Serialize a floating point number
         * 
         * @param number the number to serialize
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_number(float number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(float, std::vector<char>&)");
        }
        virtual void serialize_number(double number, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_number(double, std::vector<char>&)");
        }

        /**
         * @brief Serialize a boolean
         * 
         * @param value the boolean to serialize
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_boolean(bool value, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_boolean(bool, std::vector<char>&)");
        }

        /**
         * @brief Serialize a null value
         * 
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_null(std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_null(std::vector<char>&)");
        }

        /**
         * @brief Serialize an array
         * 
         * @param data the array to serialize
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_array(const List& data, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_array(const List&, std::vector<char>&)");
        }

        /**
         * @brief Serialize an object
         * 
         * @param data the object to serialize
         * @param buffer the buffer to store the serialized data
         */
        virtual void serialize_object(const Object& data, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("serialize_object(const Object&, std::vector<char>&)");
        }

    public:
        virtual std::vector<char> operator()(const std::unique_ptr<Object>& object) = 0;

    };

    /**
     * @brief Deserializer base class
     */
    class Deserializer {
    public:
        Deserializer() = default;
        virtual ~Deserializer() = default;

    protected:
        /**
         * @brief Deserialize a string
         * 
         * @param storage the string to store the deserialized data
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_string(std::string& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_string(std::string&, std::vector<char>&)");
        }

        /**
         * @brief Deserialize a number
         * 
         * @param storage the number to store the deserialized data
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_number(uint8_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(uint8_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(uint16_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(uint16_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(uint32_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(uint32_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(uint64_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(uint64_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(int8_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(int8_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(int16_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(int16_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(int32_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(int32_t&, std::vector<char>&)");
        }
        virtual void deserialize_number(int64_t& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(int64_t&, std::vector<char>&)");
        }

        /**
         * @brief Deserialize a floating point number
         * 
         * @param storage the number to store the deserialized data
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_number(float& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(float&, std::vector<char>&)");
        }
        virtual void deserialize_number(double& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_number(double&, std::vector<char>&)");
        }

        /**
         * @brief Deserialize a boolean
         * 
         * @param storage the boolean to store the deserialized data
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_boolean(bool& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_boolean(bool&, std::vector<char>&)");
        }

        /**
         * @brief Deserialize a null value
         * 
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_null(std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_null(std::vector<char>&)");
        }

        /**
         * @brief Deserialize an array
         * 
         * @param storage the array to store the deserialized data
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_array(List& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_array(List&, std::vector<char>&)");
        }

        /**
         * @brief Deserialize an object
         * 
         * @param storage the object to store the deserialized data
         * @param buffer the buffer to read the serialized data from
         */
        virtual void deserialize_object(Object& storage, std::vector<char>& buffer){
            throw JSONJay::MissingImplementationException("deserialize_object(Object&, std::vector<char>&)");
        }

    public:
        virtual std::unique_ptr<Object> operator()(const std::vector<char>& buffer) = 0;

    };

} // namespace JSONJay
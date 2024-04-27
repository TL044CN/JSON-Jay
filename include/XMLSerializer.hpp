/**
 * @file XMLSerializer.hpp
 * @author TL044CN
 * @brief XMLSerializer class header file
 * @version 0.1
 * @date 2024-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "Serializer.hpp"

#include "List.hpp"
#include "Object.hpp"

namespace JSONJay {

    /**
     * @brief XMLSerializer class
     */
    class XMLSerializer : public Serializer {
    public:
        XMLSerializer() = default;

    protected:
        void serialize_string(const std::string& data, std::vector<char>& buffer) override;
        void serialize_number(uint8_t number, std::vector<char>& buffer) override;
        void serialize_number(uint16_t number, std::vector<char>& buffer) override;
        void serialize_number(uint32_t number, std::vector<char>& buffer) override;
        void serialize_number(uint64_t number, std::vector<char>& buffer) override;
        void serialize_number(int8_t number, std::vector<char>& buffer) override;
        void serialize_number(int16_t number, std::vector<char>& buffer) override;
        void serialize_number(int32_t number, std::vector<char>& buffer) override;
        void serialize_number(int64_t number, std::vector<char>& buffer) override;
        void serialize_number(float number, std::vector<char>& buffer) override;
        void serialize_number(double number, std::vector<char>& buffer) override;
        void serialize_boolean(bool value, std::vector<char>& buffer) override;
        void serialize_null(std::vector<char>& buffer) override;
        void serialize_array(const List& list, std::vector<char>& buffer) override;
        void serialize_object(const Object& object, std::vector<char>& buffer) override;

    public:
        /**
         * @brief Serialize an object
         * 
         * @param object the object to serialize
         * @return std::string the serialized object
         */
        std::vector<char> serialize(const std::unique_ptr<Object>& object);

    };



} // namespace JSONJay
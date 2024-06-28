/**
 * @file Object.hpp
 * @author TL044CN
 * @brief Object class header file
 * @version 0.3
 * @date 2024-04-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Common.hpp"
#include "Exceptions.hpp"

#include <variant>
#include <string>
#include <map>

namespace JSONJay {

class List;
class Object;

/**
 * @ingroup StorageClasses
 * @brief Object class
 * @details The Object class is used to store key-value pairs. The keys are
 *          strings and the values can be any of the supported data types.
 * @see IsValidDataType
 * @see IsValidPtrDataType
 * @see BaseDataType
 */
class Object {
protected:
    /**
     * @brief The internal data type of the Object
     */
    using data_t = std::variant<std::string, int, double, bool, Object*, List*, std::monostate>;

private:
    std::map<std::string, data_t> mData;

public:
    Object() = default;
    Object(Object&&) = default;

    virtual ~Object();

private:
    /**
     * @brief check if a key is valid or throw on invalid key
     *
     * @param key               the key
     * @param bThrow            if true, throw an exception if the key is invalid
     */
    bool check_key_valid(const std::string& key, bool bThrow = true) const;

    /**
     * @brief check if a key exists or throw on collision
     *
     * @param key               the key
     * @param bThrow            if true, throw an exception if the key does not exist,
     *                          or if it exists and bThrowCollision is true
     * @param bThrowCollision   if true, throw an exception if the key already exists
     */
    bool check_key_exists(const std::string& key, bool bThrow = false, bool bThrowCollision = false) const;

public:

    /**
     * @brief Get the value at a key
     *
     * @tparam T the type of the value
     * @param key the key
     * @param value the default value
     * @return T& the value
     */
    template<typename T>
        requires IsValidDataType<T> || IsValidPtrDataType<T>
    T& at(const std::string& key) {
        check_key_exists(key, true);
        if constexpr ( IsValidPtrDataType<T> ) {
            if(!std::holds_alternative<T*>(mData.at(key)))
                throw InvalidTypeException("Invalid type");
            return *std::get<T*>(mData.at(key));
        } else {
            if(!std::holds_alternative<T>(mData.at(key)))
                throw InvalidTypeException("Invalid type");
            return std::get<T>(mData.at(key));
        }
    }

    /**
     * @brief Get the value at a key
     *
     * @tparam T the type of the value
     * @param key the key
     * @return T& the value
     */
    template<typename T>
        requires IsValidDataType<T> || IsValidPtrDataType<T>
    T& operator[](const std::string& key) {
        return at<T>(key, T());
    }

    /**
     * @brief Set a value
     *
     * @tparam T the type of the value
     * @param key the key
     * @param value the value
     */
    template<typename T>
        requires IsValidDataType<T>
    void set(const std::string& key, const T& value) {
        check_key_valid(key, true);
        if ( check_key_exists(key) ) at<T>(key) = value;
        else mData.insert({ key, data_t(value) });
    }

    /**
     * @brief Set a value
     *
     * @tparam T the type of the value
     * @param key the key
     * @param value the value
     */
    template<typename T>
        requires IsValidPtrDataType<T>
    void set(const std::string& key, T&& value) {
        check_key_valid(key, true);
        if ( check_key_exists(key) ) at<T*>(key) = value;
        else mData.insert({ key, data_t(new T(value)) });
    }

    /**
     * @brief Get the type of an element
     *
     * @param key the key of the element
     * @return BaseDataType the type of the element
     */
    inline BaseDataType get_type(const std::string& key) const {
        check_key_exists(key, true);
        auto visitor = [](auto&& arg) -> BaseDataType {
            using T = std::decay_t<decltype(arg)>;
            if      constexpr ( std::is_same_v<T, std::string> )    return BaseDataType::STRING;
            else if constexpr ( std::is_same_v<T, int> )            return BaseDataType::INT;
            else if constexpr ( std::is_same_v<T, double> )         return BaseDataType::DOUBLE;
            else if constexpr ( std::is_same_v<T, bool> )           return BaseDataType::BOOL;
            else if constexpr ( std::is_same_v<T, Object*> )        return BaseDataType::OBJECT;
            else if constexpr ( std::is_same_v<T, List*> )          return BaseDataType::LIST;
            else if constexpr ( std::is_same_v<T, std::monostate> ) return BaseDataType::NONE;
            throw InvalidTypeException("Invalid type");
        };

        // This is a hack to remove the const qualifier.
        // This is safe because we are not modifying the data.
        auto& nonConstData = const_cast<std::map<std::string, data_t>&>(mData);
        auto& nonConstElement = nonConstData.at(key);

        return JSONJay::get_type(nonConstElement);
    }

    /**
     * @brief Get a string value
     *
     * @param key the key
     * @return std::string the value
     */
    std::string& get_string(const std::string& key);

    /**
     * @brief Get an integer value
     *
     * @param key the key
     * @return int the value
     */
    int& get_int(const std::string& key);

    /**
     * @brief Get a double value
     *
     * @param key the key
     * @return double the value
     */
    double& get_double(const std::string& key);

    /**
     * @brief Get a boolean value
     *
     * @param key the key
     * @return bool the value
     */
    bool& get_bool(const std::string& key);

    /**
     * @brief Get an object value
     *
     * @param key the key
     * @return Object the value
     */
    Object& get_object(const std::string& key);

    /**
     * @brief Get a list value
     *
     * @param key the key
     * @return List the value
     */
    List& get_list(const std::string& key);

};

} // namespace JSONJay
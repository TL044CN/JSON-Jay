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
    /**
     * @brief Construct a new Object object
     */
    Object() = default;

    /**
     * @brief Construct a new Object object
     *
     * @param object the object to copy
     */
    Object(Object&&) = default;

    /**
     * @brief Destroy the Object object
     */
    virtual ~Object();

public:
    /**
     * @brief Iterator Class of the Object
     * @details Iterates over the key-value pairs of the object and additionally
     *          returns the type of the value
     */
    class Iterator {
    private:
        std::map<std::string,data_t>::iterator mIt;                         ///< The base iterator

    public:
        using iterator_category = std::forward_iterator_tag;                ///< The iterator category
        using value_type = std::tuple<std::string, data_t&, BaseDataType>;  ///< The value type

        /**
         * @brief Construct a new Iterator object
         *
         * @param it the iterator to copy
         */
        explicit Iterator(std::map<std::string,data_t>::iterator it);

        /**
         * @brief Increment the iterator
         *
         * @return Iterator& the iterator
         */
        Iterator& operator++();

        /**
         * @brief Compare two iterators
         *
         * @param other the other iterator
         * @return true if the iterators are equal
         * @return false if the iterators are not equal
         */
        bool operator!=(const Iterator& other) const;

        /**
         * @brief Dereference the iterator
         * @details Returns a tuple containing the key, the value and the type
         *          of the value.<br>
         *          The type is determined using JSONJay::get_type function.<br>
         *          The value is a reference to the actual value in the object.<br>
         *          This means that the value can be modified using the iterator.<br>
         *          The type is a BaseDataType enum value.<br>
         *          The key is a string. 
         * @return value_type the value of the iterator
         */
        value_type operator*();

    };

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
     * @brief Get the begin iterator of the list
     *
     * @return Iterator the begin iterator
     */
    Iterator begin();

    /**
     * @brief Get the end iterator of the list
     *
     * @return Iterator the end iterator
     */
    Iterator end();


    /**
     * @brief weather the object is empty
     * 
     * @return true object is empty
     * @return false object is not empty
     */
    bool empty() const;

    /**
     * @brief Get the size of the object
     *
     * @return uint64_t the size of the object
     */
    size_t size() const;

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
     * @brief Get the value at a key
     *
     * @tparam T the type of the value
     * @param key the key
     * @return T& the value
     */
    template<typename T>
        requires IsValidDataType<T> || IsValidPtrDataType<T>
    T& at(const char* key) {
        return at<T>(std::string(key));
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
    void set(const char* key, const char* value) {
        set(std::string(key), std::string(value));
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

    void erase(const std::string& key);

    /**
     * @brief Get the type of an element
     *
     * @param key the key of the element
     * @return BaseDataType the type of the element
     */
    inline BaseDataType get_type(const std::string& key) const {
        check_key_exists(key, true);

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
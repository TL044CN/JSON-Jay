/**
 * @file Object.hpp
 * @author TL044CN
 * @brief Object class header file
 * @version 0.1
 * @date 2024-04-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Exceptions.hpp"

#include <variant>
#include <string>
#include <map>

namespace JSONJay {

class List;
class Object;

/**
 * @brief Concept for checking if a type is a object type
 *
 * @tparam T the type to check
 */
template <typename T>
concept IsObjectType = std::disjunction_v<
    std::is_same<T, std::string>,
    std::is_same<T, int>,
    std::is_same<T, double>,
    std::is_same<T, bool>,
    std::is_same<T, Object*>,
    std::is_same<T, List*>,
    std::is_same<T, std::monostate>
>;

/**
 * @brief Concept for checking if a type is stored as pointer in Object
 *
 * @tparam T the type to check
 */
template <typename T>
concept IsObjectPtrType = std::disjunction_v<
    std::is_same<T, Object>,
    std::is_same<T, List>
>;

/**
 * @brief Virtual Object base class
 * @note Every object in the JSONJay library must inherit from this class
 * @details This class is the base class for all objects in the JSONJay library.
 *          It provides a common interface for all objects.
 *          An Object is a map of key-value pairs, where the key is a string
 *          The inherited classes must manage the data in their own way.
 *          Another important class is the List class, which is used to store
 *          lists of values.
 * @see XMLObject
 * @see JSONObject
 * @see List
 */
class Object {
public:
    /**
     * @brief Enum class for the data type of the list
     */
    enum class DataType {
        STRING,     ///<< The element is a string
        INT,        ///<< The element is an integer
        DOUBLE,     ///<< The element is a double
        BOOL,       ///<< The element is a boolean
        OBJECT,     ///<< The element is an object
        LIST,       ///<< The element is a list
        NONE        ///<< The element is empty
    };

protected:
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
    requires IsObjectType<T> || IsObjectPtrType<T>
    T& at(const std::string& key) {
        check_key_exists(key, true);
        if constexpr ( IsObjectPtrType<T> ) {
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
        requires IsObjectType<T> || IsObjectPtrType<T>
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
        requires IsObjectType<T> || IsObjectPtrType<T>
    void set(const std::string& key, const T& value) {
        check_key_valid(key, true);
        if constexpr ( IsObjectPtrType<T> ) {
            if ( check_key_exists(key) ) delete std::get<T*>(mData.at(key));
            mData.insert({ key, data_t(new T(value)) });
        } else {
            if ( check_key_exists(key) ) at<T>(key) = value;
            else mData.insert({ key, data_t(value) });
        }
    }

    /**
     * @brief Get the type of an element
     *
     * @param index the index of the element
     * @return DataType the type of the element
     */
    inline DataType get_type(const std::string& key) const {
        check_key_exists(key, true);
        auto visitor = [](auto&& arg) -> DataType {
            using T = std::decay_t<decltype(arg)>;
            if      constexpr ( std::is_same_v<T, std::string> )    return DataType::STRING;
            else if constexpr ( std::is_same_v<T, int> )            return DataType::INT;
            else if constexpr ( std::is_same_v<T, double> )         return DataType::DOUBLE;
            else if constexpr ( std::is_same_v<T, bool> )           return DataType::BOOL;
            else if constexpr ( std::is_same_v<T, Object*> )        return DataType::OBJECT;
            else if constexpr ( std::is_same_v<T, List*> )          return DataType::LIST;
            else if constexpr ( std::is_same_v<T, std::monostate> ) return DataType::NONE;
            throw InvalidTypeException("Invalid type");
            };

        // This is a hack to remove the const qualifier.
        // This is safe because we are not modifying the data.
        auto& nonConstData = const_cast<std::map<std::string, data_t>&>(mData);
        auto& nonConstElement = nonConstData.at(key);

        return std::visit(visitor, nonConstElement);
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
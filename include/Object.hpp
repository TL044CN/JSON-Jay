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

#include <variant>
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace JSONJay {

class List;

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
protected:
    enum class DataType {
        STRING,
        INT,
        DOUBLE,
        BOOL,
        OBJECT,
        LIST,
        NONE
    };

public:
    Object() = default;
    virtual ~Object() = default;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, const std::string& value) = 0;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, int value) = 0;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, double value) = 0;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, bool value) = 0;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, const Object& value) = 0;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, const List& value) = 0;

    /**
     * @brief Set a value
     * 
     * @param key the key
     * @param value the value
     */
    virtual void set(const std::string& key, std::monostate value) = 0;

    /**
     * @brief Get the type of a value
     * 
     * @param key the key
     * @return DataType the type of the value
     */
    virtual DataType get_type(const std::string& key) const = 0;

    /**
     * @brief Get a string value
     * 
     * @param key the key
     * @return std::string the value
     */
    virtual std::string get_string(const std::string& key) const = 0;

    /**
     * @brief Get an integer value
     * 
     * @param key the key
     * @return int the value
     */
    virtual int get_int(const std::string& key) const = 0;

    /**
     * @brief Get a double value
     * 
     * @param key the key
     * @return double the value
     */
    virtual double get_double(const std::string& key) const = 0;
    
    /**
     * @brief Get a boolean value
     * 
     * @param key the key
     * @return bool the value
     */
    virtual bool get_bool(const std::string& key) const = 0;

    /**
     * @brief Get an object value
     * 
     * @param key the key
     * @return Object the value
     */
    virtual std::reference_wrapper<Object> get_object(const std::string& key) const = 0;

    /**
     * @brief Get a list value
     * 
     * @param key the key
     * @return List the value
     */
    virtual std::reference_wrapper<List> get_list(const std::string& key) const = 0;

    /**
     * @brief Serialize the object
     * 
     * @param buffer the buffer to store the serialized data
     */
    virtual void serialize(std::vector<char>& buffer) const = 0;

};

} // namespace JSONJay
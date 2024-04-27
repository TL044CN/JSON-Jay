/**
 * @file List.hpp
 * @author TL044CN
 * @brief  List class header file
 * @version 0.1
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <variant>
#include <string>
#include <vector>

namespace JSONJay {

class Object;

/**
 * @brief List class
 */
class List {
private:
    using data_t = std::variant<std::string, int, double, bool, Object, List, std::monostate>;

    enum class DataType {
        STRING,
        INT,
        DOUBLE,
        BOOL,
        OBJECT,
        LIST,
        NONE
    };

    std::vector<data_t> mData;

public:
    List() = default;
    ~List() = default;

    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(const std::string& value);

    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(int value);
    
    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(double value);

    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(bool value);

    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(const Object& value);

    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(const List& value);

    /**
     * @brief Push a value to the list
     * 
     * @param value the value to push
     */
    void push_back(std::monostate value);

    /**
     * @brief Get the size of the list
     * 
     * @return size_t the size of the list
     */
    size_t size() const;

    /**
     * @brief Access an element of the list
     * 
     * @param index the index of the element
     * @return data_t& the element
     */
    data_t& operator[](size_t index);

    /**
     * @brief Access an element of the list
     * 
     * @param index the index of the element
     * @return const data_t& the element
     */
    const data_t& operator[](size_t index) const;

    /**
     * @brief Clear the list
     */
    void clear();

    /**
     * @brief Check if the list is empty
     * 
     * @return true if the list is empty
     * @return false if the list is not empty
     */
    bool empty() const;

    /**
     * @brief Erase an element from the list
     * 
     * @param index the index of the element to erase
     */
    void erase(size_t index);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, const std::string& value);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, int value);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, double value);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, bool value);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, const Object& value);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, const List& value);

    /**
     * @brief Insert a value to the list
     * 
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, std::monostate value);

    /**
     * @brief Get the type of an element
     * 
     * @param index the index of the element
     * @return DataType the type of the element
     */
    DataType get_type(size_t index) const;

    /**
     * @brief Get a string from the list
     * 
     * @param index the index of the string
     * @return std::string the string
     */
    std::string get_string(size_t index) const;

    /**
     * @brief Get an integer from the list
     * 
     * @param index the index of the integer
     * @return int the integer
     */
    int get_int(size_t index) const;

    /**
     * @brief Get a double from the list
     * 
     * @param index the index of the double
     * @return double the double
     */
    double get_double(size_t index) const;

    /**
     * @brief Get a boolean from the list
     * 
     * @param index the index of the boolean
     * @return bool the boolean
     */
    bool get_bool(size_t index) const;

    /**
     * @brief Get an object from the list
     * 
     * @param index the index of the object
     * @return Object the object
     */
    Object get_object(size_t index) const;

    /**
     * @brief Get a list from the list
     * 
     * @param index the index of the list
     * @return List the list
     */
    List get_list(size_t index) const;

};

} // namespace JSONJay

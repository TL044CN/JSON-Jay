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

#include "Exceptions.hpp"

#include <variant>
#include <string>
#include <vector>
#include <concepts>
#include <type_traits>

namespace JSONJay {

class Object;
class List;

/**
 * @brief Concept for checking if a type is a list type
 *
 * @tparam T the type to check
 */
template <typename T>
concept IsListType = std::disjunction_v<
    std::is_same<T, std::string>,
    std::is_same<T, int>,
    std::is_same<T, double>,
    std::is_same<T, bool>,
    std::is_same<T, Object*>,
    std::is_same<T, List*>,
    std::is_same<T, std::monostate>
>;

/**
 * @brief Concept for checking if a type is stored as pointer in List
 *
 * @tparam T the type to check
 */
template <typename T>
concept IsListPtrType = std::disjunction_v<
    std::is_same<T, Object>,
    std::is_same<T, List>
>;

/**
 * @brief List class
 */
class List {
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
    std::vector<data_t> mData;

public:
    List() = default;
    List(List&&) = default;
    ~List();

private:
    /**
     * @brief Check if the index is in bounds
     *
     * @param index the index to check
     */
    void check_index(size_t index) const;

public:
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
    void push_back(Object&& value);

    /**
     * @brief Push a value to the list
     *
     * @param value the value to push
     */
    void push_back(List&& value);

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
     * @return const data_t& the element
     */
    template<typename T>
        requires IsListType<T> || IsListPtrType<T>
    T& at(size_t index) {
        check_index(index);
        if constexpr ( IsListPtrType<T> ) {
            if ( !std::holds_alternative<T*>(mData[index]) )
                throw InvalidTypeException("Invalid type");
            return *std::get<T*>(mData.at(index));
        } else {
            if ( !std::holds_alternative<T>(mData[index]) )
                throw InvalidTypeException("Invalid type");
            return std::get<T>(mData.at(index));
        }

    }

    /**
     * @brief Access an element of the list
     *
     * @param index the index of the element
     * @return data_t& the element
     */
    const data_t& operator[](size_t index) const {
        check_index(index);
        return mData[index];
    }

    /**
     * @brief Access an element of the list
     *
     * @tparam T the type of the element
     * @param index the index of the element
     * @return const T& the element
     */
    template <typename T>
        requires IsListType<T> || IsListPtrType<T>
    T& operator[](size_t index) {
        return at<T>(index);
    }

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
    void insert(size_t index, Object&& value);

    /**
     * @brief Insert a value to the list
     *
     * @param index the index to insert the value
     * @param value the value to insert
     */
    void insert(size_t index, List&& value);

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
    inline DataType get_type(size_t index) const {
        check_index(index);
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
        auto& nonConstData = const_cast<std::vector<data_t>&>(mData);
        auto& nonConstElement = nonConstData.at(index);

        return std::visit(visitor, nonConstElement);
    }

    /**
     * @brief Get a string from the list
     *
     * @param index the index of the string
     * @return std::string the string
     */
    std::string& get_string(size_t index);

    /**
     * @brief Get an integer from the list
     *
     * @param index the index of the integer
     * @return int the integer
     */
    int& get_int(size_t index);

    /**
     * @brief Get a double from the list
     *
     * @param index the index of the double
     * @return double the double
     */
    double& get_double(size_t index);

    /**
     * @brief Get a boolean from the list
     *
     * @param index the index of the boolean
     * @return bool the boolean
     */
    bool& get_bool(size_t index);

    /**
     * @brief Get an object from the list
     *
     * @param index the index of the object
     * @return Object the object
     */
    Object& get_object(size_t index);

    /**
     * @brief Get a list from the list
     *
     * @param index the index of the list
     * @return List the list
     */
    List& get_list(size_t index);

};

} // namespace JSONJay

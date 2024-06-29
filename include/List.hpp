/**
 * @file List.hpp
 * @author TL044CN
 * @brief  List class header file
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
#include <vector>
#include <concepts>
#include <type_traits>

namespace JSONJay {

/**
 * @ingroup StorageClasses
 * @brief List class
 * @details The List class is used to store a list of values. The values can be
 *          any of the supported data types.
 * @see IsValidDataType
 * @see IsValidPtrDataType
 * @see BaseDataType
 */
class List {
protected:
    /**
     * @brief The internal data type of the list
     */
    using data_t = std::variant<std::string, int, double, bool, Object*, List*, std::monostate>;

private:
    std::vector<data_t> mData;

public:
    /**
     * @brief Construct a new List object
     */
    List() = default;

    /**
     * @brief Construct a new List object
     *
     * @param list the list to copy
     */
    List(List&&) = default;

    /**
     * @brief Destroy the List object
     */
    ~List();

private:
    /**
     * @brief Check if the index is in bounds
     *
     * @param index the index to check
     */
    void check_index(size_t index) const;

    /**
     * @brief Iterator Class of the List
     */
    class Iterator {
    private:
        std::vector<data_t>::iterator mIt;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<data_t&, BaseDataType>;

        /**
         * @brief Construct a new Iterator object
         *
         * @param it the iterator to copy
         */
        explicit Iterator(std::vector<data_t>::iterator it);

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
         *
         * @return value_type the value of the iterator
         */
        value_type operator*();
    };

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
     * @brief Push a value to the list
     *
     * @tparam T the type of the value
     * @param value the value to push
     */
    template<typename T>
        requires IsValidDataType<T>
    void push_back(T value) {
        mData.push_back(data_t(value));
    }

    /**
     * @brief Push a value to the list
     *
     * @tparam T the type of the value
     * @param value the value to push
     */
    template<typename T>
        requires IsValidPtrDataType<T>
    void push_back(T&& value) {
        mData.push_back(new T(std::move(value)));
    }

    /**
     * @brief Push a string to the list
     *
     * @param value the string to push
     */
    void push_back(const char* value) {
        mData.push_back(std::string(value));
    }

    /**
     * @brief Get the size of the list
     *
     * @return size_t the size of the list
     */
    size_t size() const;

    /**
     * @brief Access an element of the list
     *
     * @tparam T the type of the element
     * @param index the index of the element
     * @return T& the element
     */
    template<typename T>
        requires IsValidDataType<T> || IsValidPtrDataType<T>
    T& at(size_t index) {
        check_index(index);
        if constexpr ( IsValidPtrDataType<T> ) {
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
        requires IsValidDataType<T> || IsValidPtrDataType<T>
    T & operator[](size_t index) {
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
     * @tparam T the type of the value
     * @param index the index to insert the value
     * @param value the value to insert
     */
    template<typename T>
        requires IsValidDataType<T>
    void insert(size_t index, T value) {
        check_index(index);
        mData.insert(mData.begin() + index, value);
    }

    /**
     * @brief Insert a value to the list
     *
     * @tparam T the type of the value
     * @param index the index to insert the value
     * @param value the value to insert
     */
    template<typename T>
        requires IsValidPtrDataType<T>
    void insert(size_t index, T&& value) {
        check_index(index);
        mData.insert(mData.begin() + index, new T(std::move(value)));
    }

    /**
     * @brief Get the type of an element
     *
     * @param index the index of the element
     * @return BaseDataType the type of the element
     */
    inline BaseDataType get_type(size_t index) const {
        check_index(index);
        
        // This is a hack to remove the const qualifier.
        // This is safe because we are not modifying the data.
        auto& nonConstData = const_cast<std::vector<data_t>&>(mData);
        auto& nonConstElement = nonConstData.at(index);

        return JSONJay::get_type(nonConstElement);
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

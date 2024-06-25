/**
 * @file Common.hpp
 * @author TL044CN
 * @brief Common header file for the JSONJay library
 * @version 0.2
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <concepts>
#include <string>
#include <variant>

namespace JSONJay {

class StreamWriter;
class StreamReader;

/**
 * @brief checks if a type is serializable
 * @tparam T the type to check
 */
template<typename T>
concept IsSerializable = requires(StreamWriter& writer, const T& t) {
    { T::serialize(writer, t) };
};

/**
 * @brief Checks if a type is deserializable
 * @tparam T the type to check
 */
template<typename T>
concept IsDeserializable = requires(StreamReader& reader, T& t) {
    { T::deserialize(reader, t) };
};

/**
 * @brief Concept for checking if a type is numerical
 *
 * @tparam T the type to check
 */
template <typename T>
concept numerical = std::integral<T> || std::floating_point<T>;

class Object;
class List;

/**
 * @brief Concept for checking if a type is a valid Storage Class data type
 *
 * @tparam T the type to check
 */
template <typename T>
concept IsValidDataType = std::disjunction_v<
    std::is_same<T, std::string>,
    std::is_same<T, int>,
    std::is_same<T, double>,
    std::is_same<T, bool>,
    std::is_same<T, Object*>,
    std::is_same<T, List*>,
    std::is_same<T, std::monostate>
>;

/**
 * @brief Concept for checking if a type is stored as pointer in the Storage Classes
 *
 * @tparam T the type to check
 */
template <typename T>
concept IsValidPtrDataType = std::disjunction_v<
    std::is_same<T, Object>,
    std::is_same<T, List>
>;

/**
 * @brief Enum class for the data type of the Storage Classes
 */
enum class BaseDataType {
    STRING,     ///< The element is a string
    INT,        ///< The element is an integer
    DOUBLE,     ///< The element is a double
    BOOL,       ///< The element is a boolean
    OBJECT,     ///< The element is an object
    LIST,       ///< The element is a list
    NONE        ///< The element is empty
};


} // namespace JSONJay
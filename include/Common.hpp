/**
 * @file Common.hpp
 * @author TL044CN
 * @brief Common header file for the JSONJay library
 * @version 0.3
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

class StreamWritinator;
class StreamReadinator;

/**
 * @interface IsSerializable
 * @ingroup Serialization
 * @brief checks if a type is serializable
 * @tparam T the type to check
 */
template<typename T>
concept IsSerializable = requires(StreamWritinator & writer, const T & t) {
    { T::serialize(writer, t) };
};

/**
 * @interface IsDeserializable
 * @ingroup Serialization
 * @brief Checks if a type is deserializable
 * @tparam T the type to check
 */
template<typename T>
concept IsDeserializable = requires(StreamReadinator & reader, T & t) {
    { T::deserialize(reader, t) };
};

/**
 * @interface numerical
 * @brief Concept for checking if a type is numerical
 * @tparam T the type to check
 */
template <typename T>
concept numerical = std::integral<T> || std::floating_point<T>;

class Object;
class List;

/**
 * @interface IsValidDataType
 * @ingroup StorageClasses
 * @brief Concept for checking if a type is a valid Storage Class data type
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
 * @interface IsValidPtrDataType
 * @ingroup StorageClasses
 * @brief Concept for checking if a type is stored as pointer in the Storage Classes
 * @tparam T the type to check
 */
template <typename T>
concept IsValidPtrDataType = std::disjunction_v<
    std::is_same<T, Object>,
    std::is_same<T, List>
>;

/**
 * @ingroup StorageClasses
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

/**
 * @ingroup StorageClasses
 * @brief the storage type for the Storage Classes
 */
using storage_t = std::variant<std::string, int, double, bool, Object*, List*, std::monostate>;

/**
 * @brief Get the type of the data
 * 
 * @param data the data to get the type of
 * @return BaseDataType the type of the data
 */
BaseDataType get_type(storage_t& data);

} // namespace JSONJay
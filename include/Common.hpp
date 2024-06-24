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

template <typename T>
concept numerical = std::integral<T> || std::floating_point<T>;

} // namespace JSONJay
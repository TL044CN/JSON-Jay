/**
 * @file Common.hpp
 * @author TL044CN
 * @brief Common header file for the JSONJay library
 * @version 0.1
 * @date 2024-04-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <concepts>

namespace JSONJay {

template <typename T>
concept numerical = std::integral<T> || std::floating_point<T>;

} // namespace JSONJay
/**
 * @file StreamWriter.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief abstract StreamWriter base class
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <unordered_map>

#include "Common.hpp"

namespace JSONJay {

/**
 * @ingroup Serialization
 * @brief abstract StreamWriter base class
 * @details The StreamWriter class is an abstract base class for writing
 *          data to a stream. The stream can be a file, a network socket
 *          or any other destination for data.
 */
class StreamWriter {
public:
    /**
     * @brief Destroy the StreamWriter object
     */
    virtual ~StreamWriter() = default;

    /**
     * @brief checks if the stream is good
     *
     * @return true stream is good
     * @return false stream is bad
     */
    virtual bool isStreamGood() const = 0;

    /**
     * @brief returns the current stream position
     *
     * @return uint64_t the current stream position
     */
    virtual uint64_t getStreamPosition() const = 0;

    /**
     * @brief sets the stream position
     *
     * @param position the position to set
     */
    virtual void setStreamPosition(uint64_t position) = 0;

    /**
     * @brief writes data to the stream
     *
     * @param data the data to write
     * @param size the size of the data
     * @return true write successful
     * @return false write failed
     */
    virtual bool writeData(const char* data, uint64_t size) = 0;

    /**
     * @brief check if the stream is good
     *
     * @return true stream is good
     * @return false stream is bad
     */
    operator bool() const {
        return isStreamGood();
    }

    /**
     * @brief writes a buffer to the stream
     *
     * @param buffer the buffer to write
     * @param writeSize whether to write the size of the buffer
     */
    void writeBuffer(const std::vector<char>& buffer, bool writeSize = true);

    /**
     * @brief writes a zero to the stream
     *
     * @param size the size of the zero
     */
    void writeZero(uint64_t size);

    /**
     * @brief writes a string to the stream
     *
     * @param str the string to write
     */
    void writeString(const std::string& str);

    /**
     * @brief writes a string to the stream
     *
     * @param str the string to write
     */
    void writeString(std::string_view str);

    /**
     * @brief writes a raw data to the stream
     *
     * @tparam T the type of the data
     * @param data the data to write
     */
    template<typename T>
    void writeRaw(const T& data) {
        writeData(reinterpret_cast<const char*>(&data), sizeof(T));
    }

    /**
     * @brief writes a serializable object to the stream
     *
     * @tparam T the type of the object
     * @param data the object to write
     */
    template<typename T>
        requires IsSerializable<T>
    void writeSerializable(const T& data) {
        T::serialize(*this, data);
    }

    /**
     * @brief writes a map to the stream
     * @note the key and value types must be trivial, serializable or a string
     *
     * @tparam K the type of the key
     * @tparam V the type of the value
     * @param map the map to write
     * @param writeSize whether to write the size of the map
     */
    template<typename K, typename V>
        requires (std::is_trivial_v<K> || IsSerializable<K> || std::is_same_v<K, std::string>) &&
                 (std::is_trivial_v<V> || IsSerializable<V> || std::is_same_v<K, std::string>)
        void writeMap(const std::map<K, V>& map, bool writeSize = true) {
        if ( writeSize )
            writeRaw<uint32_t>(map.size());
        for ( const auto& [key, value] : map ) {
            if constexpr ( std::is_trivial<K>() )
                writeRaw(key);
            else if constexpr ( std::is_same_v<K, std::string> )
                writeString(key);
            else
                writeSerializable(key);

            if constexpr ( std::is_trivial<V>() )
                writeRaw(value);
            else if constexpr ( std::is_same_v<V, std::string> )
                writeString(value);
            else
                writeSerializable(value);
        }
    }

    /**
     * @brief writes an unordered map to the stream
     * @note the key and value types must be trivial, serializable or a string
     *
     * @tparam K the type of the key
     * @tparam V the type of the value
     * @param map the map to write
     * @param writeSize whether to write the size of the map
     */
    template<typename K, typename V>
        requires (std::is_trivial_v<K> || IsSerializable<K> || std::is_same_v<K, std::string>) &&
                 (std::is_trivial_v<V> || IsSerializable<V> || std::is_same_v<K, std::string>)
    void writeMap(const std::unordered_map<K, V>& map, bool writeSize = true) {
        if ( writeSize )
            writeRaw<uint32_t>(map.size());
        for ( const auto& [key, value] : map ) {
            if constexpr ( std::is_trivial<K>() )
                writeRaw(key);
            else if constexpr ( std::is_same_v<K, std::string> )
                writeString(key);
            else
                writeSerializable(key);

            if constexpr ( std::is_trivial<V>() )
                writeRaw(value);
            else if constexpr ( std::is_same_v<V, std::string> )
                writeString(value);
            else
                writeSerializable(value);
        }
    }

    /**
     * @brief writes a vector to the stream
     * @note the type must be trivial, serializable or a string
     *
     * @tparam T the type of the vector
     * @param vector the vector to write
     * @param writeSize whether to write the size of the vector
     */
    template<typename T>
        requires (std::is_trivial_v<T> || IsSerializable<T> || std::is_same_v<T, std::string>)
    void writeVector(const std::vector<T>& vector, bool writeSize = true) {
        if ( writeSize )
            writeRaw<uint32_t>(vector.size());
        for ( const auto& element : vector ) {
            if constexpr ( std::is_trivial<T>() )
                writeRaw(element);
            else if constexpr ( std::is_same_v<T, std::string> )
                writeString(element);
            else
                writeSerializable(element);
        }
    }
};

} // namespace JSONJay
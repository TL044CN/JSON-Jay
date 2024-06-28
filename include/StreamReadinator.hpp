/**
 * @file StreamReadinator.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief abstract StreamReadinator base class
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
#include <map>
#include <unordered_map>

#include "Common.hpp"

namespace JSONJay {

/**
 * @ingroup Serialization
 * @brief abstract StreamReadinator base class
 * @details The StreamReadinator class is an abstract base class for reading
 *          data from a stream. The stream can be a file, a network socket
 *          or any other source of data.
 */
class StreamReadinator {
public:
    /**
     * @brief Destroy the StreamReadinator object
     */
    virtual ~StreamReadinator() = default;

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
    virtual uint64_t getStreamPosition() = 0;

    /**
     * @brief sets the stream position
     *
     * @param position the position to set
     */
    virtual void setStreamPosition(uint64_t position) = 0;

    /**
     * @brief reads data from the stream
     *
     * @param data the buffer to read into
     * @param size the size of the buffer
     */
    virtual bool readData(char* data, uint64_t size) = 0;

    /**
     * @brief reads data from the stream until a delimiter is found
     *
     * @param data the buffer to read into
     * @param delim the delimiter to read until
     */
    virtual bool readUntil(std::vector<char>& data, char delim);

    /**
     * @brief reads data from the stream until a delimiter is found
     *
     * @param data the buffer to read into
     * @param delim the delimiter to read until
     */
    virtual bool readUntil(std::vector<char>& data, std::string delim);

    /**
     * @brief returns weather the stream is good
     * 
     * @return true the stream is good
     * @return false the stream is bad
     */
    operator bool() const {
        return isStreamGood();
    }

    /**
     * @brief reads a buffer from the stream
     * 
     * @param buffer the buffer to read into
     * @param size the size of the buffer
     * @return true read successful
     * @return false read failed
     */
    bool readBuffer(std::vector<char>& buffer, uint32_t size = 0);

    /**
     * @brief reads a string from the stream
     * 
     * @param str the string to read into
     * @return true read successful
     * @return false read failed
     */
    bool readString(std::string& str);

    /**
     * @brief reads raw data from the stream
     * 
     * @tparam T the type to read
     * @param t the data to read into
     * @return true read successful
     * @return false read failed
     */
    template<typename T>
    bool readRaw(T& t) {
        bool success = readData(reinterpret_cast<char*>(&t), sizeof(T));
        return success;
    }

    /**
     * @brief reads a deserializable object from the stream
     * 
     * @tparam T the type to read
     * @param t the object to read into
     */
    template<typename T>
        requires IsDeserializable<T>
    void readDeserializable(T& t) {
        T::deserialize(*this, t);
    }

    /**
     * @brief reads a map from the stream
     * @note the key and value types must be deserializable, trivial or a string
     * 
     * @tparam K the key type
     * @tparam V the value type
     * @param std::is_same_v<K,std::string> the map to read into
     */
    template<typename K, typename V>
        requires (IsDeserializable<K> || std::is_trivial_v<K> || std::is_same_v<K,std::string>) &&
                 (IsDeserializable<V> || std::is_trivial_v<V> || std::is_same_v<V,std::string>)
    void readMap(std::map<K,V>& map, uint32_t size = 0) {
        if (size == 0)
            readRaw(size);
        
        for (uint32_t i = 0; i < size; i++) {
            K key;
            if constexpr (std::is_trivial_v<K>)
                readRaw(key);
            else if constexpr (std::is_same_v<K, std::string>)
                readString(key);
            else 
                readDeserializable(key);

            if constexpr (std::is_trivial_v<V>)
                readRaw<V>(map[key]);
            else if constexpr (std::is_same_v<V, std::string>)
                readString(map[key]);
            else
                readDeserializable<V>(map[key]);

        }
    }

    /**
     * @brief reads an unordered map from the stream
     * @note the key and value types must be deserializable, trivial or a string
     * 
     * @tparam K the key type
     * @tparam V the value type
     * @param map the map to read into
     */
    template<typename K, typename V>
        requires (IsDeserializable<K> || std::is_trivial_v<K> || std::is_same_v<K,std::string>) &&
                 (IsDeserializable<V> || std::is_trivial_v<V> || std::is_same_v<V,std::string>)
    void readMap(std::unordered_map<K,V>& map, uint32_t size = 0) {
        if (size == 0)
            readRaw(size);
        
        for (uint32_t i = 0; i < size; i++) {
            K key;
            if constexpr (std::is_trivial_v<K>)
                readRaw(key);
            else if constexpr (std::is_same_v<K, std::string>)
                readString(key);
            else 
                readDeserializable(key);

            if constexpr (std::is_trivial_v<V>)
                readRaw<V>(map[key]);
            else if constexpr (std::is_same_v<V, std::string>)
                readString(map[key]);
            else
                readDeserializable<V>(map[key]);

        }
    }

    /**
     * @brief reads a vector from the stream
     * @note the type must be deserializable, trivial or a string
     * 
     * @tparam T the type to read
     * @param vec the vector to read into
     */
    template<typename T>
        requires (IsDeserializable<T> || std::is_trivial_v<T> || std::is_same_v<T,std::string>)
    void readVector(std::vector<T>& vec, uint32_t size = 0) {
        if (size == 0)
            readRaw(size);
        
        for (uint32_t i = 0; i < size; i++) {
            T t;
            if constexpr (std::is_trivial_v<T>)
                readRaw(t);
            else if constexpr (std::is_same_v<T, std::string>)
                readString(t);
            else
                readDeserializable(t);
            vec.push_back(t);
        }
    }

};

} // namespace JSONJay
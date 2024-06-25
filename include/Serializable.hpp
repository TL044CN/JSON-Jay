/**
 * @file Serializable.hpp
 * @author TL044CN (tl044cn@shodan.fyi)
 * @brief abstract Serializable class
 * @version 0.1
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "Common.hpp"

namespace JSONJay {
template <typename T>
class Serializable {
public:
    /**
     * @brief Serializes an object
     * 
     * @param writer the writer to write to
     * @param object the object to serialize
     */
    static void serialize(const StreamWriter& writer, const T& object) {
        object.serialize(writer);
    }

    /**
     * @brief Deserializes an object
     * 
     * @param reader the reader to read from
     * @param object the object to deserialize
     */
    static void deserialize(const StreamReader& reader, T& object) {
        object.deserialize(reader);
    }

protected:
    /**
     * @brief Serialize the object
     * 
     * @param writer the writer to write to
     */
    virtual void serialize(const StreamWriter& writer) const = 0;

    /**
     * @brief Deserialize the object
     * 
     * @param reader the reader to read from
     */
    virtual void deserialize(const StreamReader& reader) = 0;
};

} // namespace JSONJay
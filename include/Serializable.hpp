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

/**
 * @ingroup Serialization
 * @brief abstract Serializable class
 * @details The Serializable class is an abstract base class for serializing
 *          and deserializing objects. The class provides a serialize and
 *          deserialize method that must be implemented by the derived class.
 */
template <typename T>
class Serializable {
public:

    /**
     * @brief Serialize an object
     * 
     * @tparam WriterType the type of writer to use
     * @param writer the writer to write to
     * @param object the object to serialize
     */
    template <typename WriterType>
        requires std::derived_from<WriterType, StreamWritinator>
    static void serialize(StreamWritinator* writer, const T& object) {
        object.serialize(writer);
    }

    /**
     * @brief Deserializes an object
     * 
     * @tparam ReaderType the type of reader to use
     * @param reader the reader to read from
     * @param object the object to deserialize
     */
    template <typename ReaderType>
        requires std::derived_from<ReaderType, StreamReadinator>
    static void deserialize(StreamReadinator* reader, T& object) {
        object.deserialize(reader);
    }

protected:
    /**
     * @brief Serialize the object
     * 
     * @param writer the writer to write to
     */
    virtual void serialize(StreamWritinator* writer) const = 0;

    /**
     * @brief Deserialize the object
     * 
     * @param reader the reader to read from
     */
    virtual void deserialize(StreamReadinator* reader) = 0;
};

} // namespace JSONJay
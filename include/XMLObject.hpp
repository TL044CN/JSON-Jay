/**
 * @file XMLObject.hpp
 * @author TL044CN
 * @brief XMLObject Specification
 * @version 0.1
 * @date 2024-04-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <string_view>

#include "Object.hpp"
#include "List.hpp"
#include "Serializable.hpp"

namespace JSONJay {

    /**
     * @brief XMLObject class
     */
    class XMLObject : public List, public Serializable<XMLObject> {
    public:
        /**
         * @brief Tag class
         * @details Represents an XML tag with its attributes
         */
        enum class DataType {
            STRING, ///< String data type
            OBJECT  ///< Object data type
        };

    protected:
        /**
         * @brief Tag class
         * @details Represents an XML tag with its attributes
         */
        struct Tag {
            std::string name;
            std::map<std::string, std::string> attributes;

            Tag(const std::string& name, const std::map<std::string, std::string>& attributes) : name(name), attributes(attributes) {}
            Tag(const std::string& name) : name(name) {}

            /**
             * @brief Get the value of the given attribute
             * 
             * @param attribute the attribute name
             * @return std::string the value of the attribute
             */
            void addAttribute(const std::pair<std::string, std::string>& attribute) {
                attributes.insert(attribute);
            }
            
            /**
             * @brief Add an attribute to the tag
             * 
             * @param name the name of the attribute
             * @param value the value of the attribute
             */
            void addAttribute(const std::string& name, const std::string& value) {
                attributes.insert({name, value});
            }

            /**
             * @brief Remove an attribute from the tag
             * 
             * @param attribute the attribute to remove
             */
            void removeAttribute(const std::string& attribute) {
                attributes.erase(attribute);
            }
        };

        /**
         * @brief Data class
         * @details Represents the data Node in the XMLObject
         */
        struct Data {
            Tag tag;
            std::variant<std::string, std::unique_ptr<XMLObject>> data;

            Data(std::unique_ptr<XMLObject>&& data, const Tag& tag): tag(tag) {
                this->data = std::move(data);
            }

            Data(const std::string& data, const Tag& tag): tag(tag) {
                this->data = data;
            }
            
            /**
             * @brief Get the value of the given attribute
             * 
             * @param attribute the attribute name
             * @return std::string the value of the attribute
             */
            std::string getAttribute(const std::string& attribute) const {
                if(tag.attributes.find(attribute) != tag.attributes.end())
                    return tag.attributes.at(attribute);
                return "";
            }

            /**
             * @brief Add an attribute to the tag
             * 
             * @param attribute the attribute to add
             */
            void addAttribute(const std::pair<std::string, std::string>& attribute) {
                tag.addAttribute(attribute);
            }

            /**
             * @brief Add an attribute to the tag
             * 
             * @param name the name of the attribute
             * @param value the value of the attribute
             */
            void addAttribute(const std::string& name, const std::string& value) {
                tag.addAttribute(name, value);
            }

            /**
             * @brief Remove an attribute from the tag
             * 
             * @param attribute the attribute to remove
             */
            void removeAttribute(const std::string& attribute) {
                tag.attributes.erase(attribute);
            }

            /**
             * @brief Get the attribute names
             * 
             * @return std::vector<std::string_view> the attribute names
             */
            std::vector<std::string_view> getAttributeNames() const {
                std::vector<std::string_view> names;
                for(const auto& [name, value] : tag.attributes) {
                    names.push_back(name);
                }
                return names;
            }

            /**
             * @brief Get the attribute values
             * 
             * @return std::vector<std::string_view> the attribute values
             */
            DataType getDataType() const {
                return std::holds_alternative<std::string>(data) ? DataType::STRING : DataType::OBJECT;
            }

        };
    private:
        using iterator = std::vector<Data>::iterator;
        using const_iterator = std::vector<Data>::const_iterator;

        std::vector<Data> mData;
        
    public:
        XMLObject();

        /**
         * @brief   Push a new tag with a value to the object
         * 
         * @param tag the tag name of the object
         * @param value the value of the object
         */
        void push_back(const std::string& tag, const std::string& value);

        /**
         * @brief   Push a new tag with an object to the object
         * 
         * @param tag the tag name of the object
         * @param object the object
         */
        void push_back(const std::string& tag, const XMLObject& object);

        /**
         * @brief   Push a new tag with a value to the object
         * 
         * @param tag the tag struct of the object
         * @param value the value of the object
         */
        void push_back(const Tag& tag, const std::string& value);

        /**
         * @brief   Push a new tag with an object to the object
         * 
         * @param tag the tag struct of the object
         * @param object the object
         */
        void push_back(const Tag& tag, const XMLObject& object);

        /**
         * @brief   Insert a new tag with a value to the object
         * 
         * @param tag the tag name of the object
         * @param value the value of the object
         * @param index the index to insert the tag
         */
        void insert(const std::string& tag, const std::string& value, size_t index);

        /**
         * @brief   Insert a new tag with an object to the object
         * 
         * @param tag the tag name of the object
         * @param object the object
         * @param index the index to insert the tag
         */
        void insert(const std::string& tag, const XMLObject& object, size_t index);

        /**
         * @brief   Insert a new tag with a value to the object
         * 
         * @param tag the tag struct of the object
         * @param value the value of the object
         * @param index the index to insert the tag
         */
        void insert(const Tag& tag, const std::string& value, size_t index);

        /**
         * @brief   Insert a new tag with an object to the object
         * 
         * @param tag the tag struct of the object
         * @param object the object
         * @param index the index to insert the tag
         */
        void insert(const Tag& tag, const XMLObject& object, size_t index);

        /**
         * @brief   Add an attribute to a tag
         * 
         * @param attribute the attribute to add
         * @param index the index of the tag
         */
        void addAttribute(const std::pair<std::string, std::string>& attribute, size_t index);

        /**
         * @brief   Add an attribute to a tag
         * 
         * @param name the name of the attribute
         * @param value the value of the attribute
         * @param index the index of the tag
         */
        void addAttribute(const std::string& name, const std::string& value, size_t index);

        /**
         * @brief remove the tag-object pair at the specified index
         * 
         * @param index the index of the tag-object pair to remove
         */
        void erase(size_t index);

        /**
         * @brief Get the size of the object
         * 
         * @return size_t the size of the object
         */
        size_t size() const;

        /**
         * @brief Get the tag at the specified index
         * 
         * @param index the index of the tag
         * @return Tag the tag
         */
        Tag getTag(size_t index) const;

        /**
         * @brief Get the data at the specified index
         * 
         * @param index the index of the data
         * @return data_t the data
         */
        Data getData(size_t index) const;

        /**
         * @brief Get the text at the specified index
         * 
         * @param index the index of the text
         * @return std::string the text
         */
        std::string getText(size_t index) const;

        /**
         * @brief Get the object at the specified index
         * 
         * @param index the index of the object
         * @return std::reference_wrapper<const XMLObject> the object
         */
        std::reference_wrapper<const XMLObject> getObject(size_t index) const;

        /**
         * @brief Get the attribute at the specified index
         * 
         * @param index the index of the attribute
         * @return std::map<std::string, std::string> the attribute
         */
        Data& operator[](size_t index);

        /**
         * @brief Get the begin iterator
         * 
         * @return iterator the begin iterator
         */
        iterator begin();

        /**
         * @brief Get the end iterator
         * 
         * @return iterator the end iterator
         */
        iterator end();

        /**
         * @brief Get the begin iterator
         * 
         * @return const_iterator the begin iterator
         */
        const_iterator begin() const;

        /**
         * @brief Get the end iterator
         * 
         * @return const_iterator the end iterator
         */
        const_iterator end() const;

    protected:
        /**
         * @brief Serialize the object
         * 
         * @param writer the writer to write to
         */
        void serialize(const StreamWriter& writer) const override;

        /**
         * @brief Deserialize the object
         * 
         * @param reader the reader to read from
         */
        void deserialize(const StreamReader& reader) override;
    };

} // namespace JSONJay

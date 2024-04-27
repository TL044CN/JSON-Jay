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

namespace JSONJay {

    /**
     * @brief XMLObject class
     */
    class XMLObject : public List {
    private:
        using attribute_t = std::pair<std::string, std::string>;
        using data_t = std::variant<std::string, std::unique_ptr<XMLObject>>;
    protected:
        /**
         * @brief Tag class
         * @details Represents an XML tag with its attributes
         */
        struct Tag {
            std::string name;
            std::vector<attribute_t> attributes;

            Tag(const std::string& name, const std::vector<attribute_t>& attributes) : name(name), attributes(attributes) {}
            Tag(const std::string& name) : name(name) {}
        };

        struct Data {
            Tag tag;
            data_t data;

            Data(std::unique_ptr<XMLObject>&& data, const Tag& tag): tag(tag) {
                this->data = std::move(data);
            }

            Data(const std::string& data, const Tag& tag): tag(tag) {
                this->data = data;
            }

            std::string getAttribute(const std::string& attribute) const {
                for(const auto& attr : tag.attributes) {
                    if(attr.first == attribute) {
                        return attr.second;
                    }
                }
                return "";
            }

            void addAttribute(const attribute_t& attribute) {
                tag.attributes.push_back(attribute);
            }

            void removeAttribute(const std::string& attribute) {
                for(auto it = tag.attributes.begin(); it != tag.attributes.end(); ++it) {
                    if(it->first == attribute) {
                        tag.attributes.erase(it);
                        return;
                    }
                }
            }

            std::vector<std::string_view> getAttributeNames() const {
                std::vector<std::string_view> names;
                for(const auto& attr : tag.attributes) {
                    names.push_back(attr.first);
                }
                return names;
            }

        };
    private:
        using storage_t = std::vector<std::pair<Tag, data_t>>;

    private:
        storage_t mData;
        
    public:
        XMLObject();

        /**
         * @brief Get the data
         * 
         * @return const storage_t& the data
         */
        const storage_t& getData() const;

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
        void addAttribute(const attribute_t& attribute, size_t index);

        /**
         * @brief remove the tag-object pair at the specified index
         * 
         * @param index the index of the tag-object pair to remove
         */
        void erase(size_t index);

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
        data_t getData(size_t index) const;

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

    };

} // namespace JSONJay

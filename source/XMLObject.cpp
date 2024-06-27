#include "XMLObject.hpp"

namespace JSONJay {

std::string XMLObject::Data::toString() const {
    std::string result;

    result += "<" + tag.name;
    for(const auto& [key, value] : tag.attributes)
        result += " " + key + "=\"" + value + "\"";        
    result += ">";

    if(getDataType() == DataType::STRING) {
        result += std::get<std::string>(data);
    } else {
        result += std::get<std::unique_ptr<XMLObject>&>(data)->toString();
    }

    result += "</" + tag.name + ">";

    return result;
}


std::string XMLObject::Data::toInnerString() const {
    if(getDataType() == DataType::STRING) {
        return std::get<std::string>(data);
    } else {
        return std::get<std::unique_ptr<XMLObject>&>(data)->toString();
    }
}


void XMLObject::Data::serialize(const StreamWritinator& writer) const {
    writer.writeString(toString());
}

void XMLObject::Data::deserialize(const StreamReader& reader) {
    
    std::string data;
    reader.readString(data);
    std::istringstream stream(data);
    std::string tag;
    stream >> tag;
    tag = tag.substr(1, tag.size() - 1);
    std::string value;
    std::getline(stream, value, '<');
    value = value.substr(0, value.size() - 1);
    Tag newTag(tag);
    if(value.empty()) {
        std::unique_ptr<XMLObject> obj = std::make_unique<XMLObject>();
        obj->deserialize(reader);
        mData = std::move(obj);
    } else {
        mData = value;
    }
    tag = "";
}


void XMLObject::push_back(const std::string& tag, const std::string& value) {
    Tag newTag(tag);
    Data newData(value, newTag);
    mData.emplace_back(std::move(newData));
}

void XMLObject::push_back(const std::string& tag,const XMLObject& object) {
    Tag newTag(tag);
    auto objPtr = std::make_unique<XMLObject>(object);
    Data newData(std::move(objPtr), newTag);
    mData.emplace_back(std::move(newData));
}

void XMLObject::push_back(const Tag& tag, const std::string& value) {
    Tag newTag(tag);

    Data newData(value, newTag);
    mData.emplace_back(std::move(newData));
}

void XMLObject::push_back(const Tag& tag, const XMLObject& object) {
    Tag newTag(tag);
    auto objPtr = std::make_unique<XMLObject>(object);
    Data newData(std::move(objPtr), newTag);
    mData.emplace_back(std::move(newData));
}

void XMLObject::insert(const std::string& tag, const std::string& value, size_t index) {
    Tag newTag(tag);
    Data newData(value, newTag);
    mData.insert(mData.begin() + index, std::move(newData));
}

void XMLObject::insert(const std::string& tag, const XMLObject& object, size_t index) {
    Tag newTag(tag);
    auto objPtr = std::make_unique<XMLObject>(object);
    Data newData(std::move(objPtr), newTag);
    mData.insert(mData.begin() + index, std::move(newData));
}

void XMLObject::insert(const Tag& tag, const std::string& value, size_t index) {
    Tag newTag(tag);
    Data newData(value, newTag);
    mData.insert(mData.begin() + index, std::move(newData));
}

void XMLObject::insert(const Tag& tag, const XMLObject& object, size_t index) {
    Tag newTag(tag);
    auto objPtr = std::make_unique<XMLObject>(object);
    Data newData(std::move(objPtr), newTag);
    mData.insert(mData.begin() + index, std::move(newData));
}

void XMLObject::addAttribute(const std::pair<std::string, std::string>& attribute, size_t index) {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }
    mData[index].addAttribute(attribute);
}

void XMLObject::addAttribute(const std::string& key, const std::string& value, size_t index) {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }
    mData[index].addAttribute(key, value);
}

void XMLObject::erase(size_t index) {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }
    mData.erase(mData.begin() + index);
}

size_t XMLObject::size() const {
    return mData.size();
}

XMLObject::Tag XMLObject::getTag(size_t index) const {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }
    return mData[index].tag;
}

XMLObject::Data XMLObject::getData(size_t index) const {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }
    return mData[index];
}

std::string XMLObject::toString() const {
    std::string result;

    for(const auto& data : mData) {
        result += data.toString();
    }

    return result;
}

std::string XMLObject::innerString(size_t index) const {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }

    return mData[index].toInnerString();

}

std::reference_wrapper<XMLObject::Data> XMLObject::getObject(size_t index) {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }

    if(mData[index].getDataType() == DataType::STRING) {
        throw std::runtime_error("Data is not an object");
    }

    return mData[index];
}

XMLObject::Data& XMLObject::operator[](size_t index) {
    if (index >= mData.size()) {
        throw std::out_of_range("Index out of range");
    }

    return mData[index];
}

XMLObject::iterator XMLObject::begin() {
    return mData.begin();
}

XMLObject::iterator XMLObject::end() {
    return mData.end();
}

XMLObject::const_iterator XMLObject::begin() const {
    return mData.begin();
}

XMLObject::const_iterator XMLObject::end() const {
    return mData.end();
}


void XMLObject::serialize(const StreamWritinator& writer) const {
    for(const auto& data : mData) {
        writer.writeSerializable(data);
    }
}

void XMLObject::deserialize(StreamReader& reader) {
    while(reader.isGood()) {
        Data data;
        reader.readSerializable(data);
        mData.push_back(std::move(data));
    }
}

} // namespace JSONJay
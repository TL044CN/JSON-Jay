#include "Object.hpp"
#include "List.hpp"

namespace JSONJay {

Object::~Object() {
    for ( auto& [key, element] : mData ) {
        if ( std::holds_alternative<Object*>(element) ) delete std::get<Object*>(element);
        if ( std::holds_alternative<List*>(element) ) delete std::get<List*>(element);
    }
}


bool Object::check_key_valid(const std::string& key, bool bThrow) const {
    if ( bThrow ) {
        if ( check_key_valid(key, false) ) return true;
        throw InvalidKeyException("Invalid key");
    }

    if ( key.empty() ) return false;
    if ( key.find_first_of(" \t\n") != std::string::npos ) return false;
    return true;
}

bool Object::check_key_exists(const std::string& key, bool bThrow, bool bThrowCollision) const {
    if ( bThrow ) {
        bool check = check_key_exists(key);
        if ( check == bThrowCollision )
            throw InvalidKeyException(bThrowCollision?"Key already exists":"Key does not exist");
        return check;
    }
    return mData.find(key) != mData.end();
}

std::string& Object::get_string(const std::string& key) {
    return at<std::string>(key);
}

int& Object::get_int(const std::string& key) {
    return at<int>(key);
}

double& Object::get_double(const std::string& key) {
    return at<double>(key);
}

bool& Object::get_bool(const std::string& key) {
    return at<bool>(key);
}

Object& Object::get_object(const std::string& key) {
    return *at<Object*>(key);
}

List& Object::get_list(const std::string& key) {
    return *at<List*>(key);
}

}; // namespace JSONJay
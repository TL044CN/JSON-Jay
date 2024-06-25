#include "List.hpp"
#include "Object.hpp"

namespace JSONJay {

List::~List() {
    for ( auto& element : mData ) {
        if ( std::holds_alternative<Object*>(element) ) delete std::get<Object*>(element);
        if ( std::holds_alternative<List*>(element) ) delete std::get<List*>(element);
    }

}

void List::check_index(size_t index) const {
    if ( index >= mData.size() ) throw InvalidIndexException("Index out of bounds");
}

void List::push_back(const std::string& value) {
    mData.push_back(data_t(value));
}

void List::push_back(int value) {
    mData.push_back(value);
}

void List::push_back(double value) {
    mData.push_back(value);
}

void List::push_back(bool value) {
    mData.push_back(value);
}

void List::push_back(Object&& value) {
    mData.push_back(new Object(std::move(value)));
}

void List::push_back(List&& value) {
    mData.push_back(new List(std::move(value)));
}

void List::push_back(std::monostate value) {
    mData.push_back(value);
}

size_t List::size() const {
    return mData.size();
}

void List::clear() {
    mData.clear();
}

bool List::empty() const {
    return mData.empty();
}

void List::erase(size_t index) {
    check_index(index);
    mData.erase(mData.begin() + index);
}

void List::insert(size_t index, const std::string& value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

void List::insert(size_t index, int value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

void List::insert(size_t index, double value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

void List::insert(size_t index, bool value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

void List::insert(size_t index, Object&& value) {
    check_index(index);
    mData.insert(mData.begin() + index, new Object(std::move(value)));
}

void List::insert(size_t index, List&& value) {
    check_index(index);
    mData.insert(mData.begin() + index, new List(std::move(value)));
}

void List::insert(size_t index, std::monostate value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

std::string& List::get_string(size_t index) {
    check_index(index);
    return at<std::string>(index);
}

int& List::get_int(size_t index) {
    check_index(index);
    return at<int>(index);
}

double& List::get_double(size_t index) {
    check_index(index);
    return at<double>(index);
}

bool& List::get_bool(size_t index) {
    check_index(index);
    return at<bool>(index);
}

Object& List::get_object(size_t index) {
    check_index(index);
    return *at<Object*>(index);
}

List& List::get_list(size_t index) {
    check_index(index);
    return *at<List*>(index);
}

} // namespace JSONJay
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

List::Iterator::Iterator(std::vector<data_t>::iterator it) : mIt(it) {}

List::Iterator& List::Iterator::operator++() {
    ++mIt;
    return *this;
}

List::Iterator List::Iterator::operator++(int) {
    Iterator it = *this;
    ++mIt;
    return it;
}

bool List::Iterator::operator==(const Iterator& other) {
    return mIt == other.mIt;
}

List::Iterator::value_type List::Iterator::operator*() {
    return {*mIt, JSONJay::get_type(*mIt)};
}


List::Iterator List::begin() {
    return Iterator(mData.begin());
}

List::Iterator List::end() {
    return Iterator(mData.end());
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

    if(std::holds_alternative<Object*>(mData[index])) delete std::get<Object*>(mData[index]);
    if(std::holds_alternative<List*>(mData[index])) delete std::get<List*>(mData[index]);

    mData.erase(mData.begin() + index);
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
#include "List.cpp"

namespace JSONJay {

void List::check_index(size_t index) const {
    if ( index >= mData.size() )
        throw InvalidIndexException("Index out of bounds");
}

void List::push_back(const std::string& value) {
    mData.push_back(value);
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

void List::push_back(const Object& value) {
    mData.push_back(value);
}

void List::push_back(const List& value) {
    mData.push_back(value);
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

void List::insert(size_t index, const Object& value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

void List::insert(size_t index, const List& value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

void List::insert(size_t index, std::monostate value) {
    check_index(index);
    mData.insert(mData.begin() + index, value);
}

List::Datatype List::get_type(size_t index) const {
    check_index(index);

    auto visitor = [](auto&& arg) -> Datatype {
        using T = std::decay_t<decltype(arg)>;
        if constexpr ( std::is_same_v<T, std::string> )         return DataType::STRING;
        else if constexpr ( std::is_same_v<T, int> )            return DataType::INT;
        else if constexpr ( std::is_same_v<T, double> )         return DataType::DOUBLE;
        else if constexpr ( std::is_same_v<T, bool> )           return DataType::BOOL;
        else if constexpr ( std::is_same_v<T, Object> )         return DataType::OBJECT;
        else if constexpr ( std::is_same_v<T, List> )           return DataType::LIST;
        else if constexpr ( std::is_same_v<T, std::monostate> ) return DataType::NONE;
        throw InvalidTypeException("Invalid type");
    };

    return std::visit(visitor, mData[index]);
}

std::string& List::get_string(size_t index) const {
    check_index(index);

    if ( std::holds_alternative<std::string>(mData[index]) )
        return at(index);

    return "";
}

int& List::get_int(size_t index) const {
    check_index(index);

    if ( std::holds_alternative<int>(mData[index]) )
        return at(index);

    return 0;
}

double& List::get_double(size_t index) const {
    check_index(index);

    if ( std::holds_alternative<double>(mData[index]) )
        return at(index);

    return 0.0;
}

bool& List::get_bool(size_t index) const {
    check_index(index);

    if ( std::holds_alternative<bool>(mData[index]) )
        return at(index);

    return false;
}

Object& List::get_object(size_t index) const {
    check_index(index);

    if ( std::holds_alternative<Object>(mData[index]) )
        return at(index);

    return Object();
}

List& List::get_list(size_t index) const {
    check_index(index);

    if ( std::holds_alternative<List>(mData[index]) )
        return at(index);

    return List();
}

} // namespace JSONJay
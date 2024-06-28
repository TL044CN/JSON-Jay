#include "Common.hpp"
#include "Exceptions.hpp"

namespace JSONJay {

BaseDataType get_type(storage_t& data) {

    auto visitor = [](auto&& arg) -> BaseDataType {
        using T = std::decay_t<decltype(arg)>;
        if      constexpr ( std::is_same_v<T, std::string> )    return BaseDataType::STRING;
        else if constexpr ( std::is_same_v<T, int> )            return BaseDataType::INT;
        else if constexpr ( std::is_same_v<T, double> )         return BaseDataType::DOUBLE;
        else if constexpr ( std::is_same_v<T, bool> )           return BaseDataType::BOOL;
        else if constexpr ( std::is_same_v<T, Object*> )        return BaseDataType::OBJECT;
        else if constexpr ( std::is_same_v<T, List*> )          return BaseDataType::LIST;
        else if constexpr ( std::is_same_v<T, std::monostate> ) return BaseDataType::NONE;
        throw InvalidTypeException("Invalid type");
        };

    return std::visit(visitor, data);
}

} // namespace JSONJay
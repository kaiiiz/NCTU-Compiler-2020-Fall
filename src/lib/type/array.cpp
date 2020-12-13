#include <string>
#include <memory>

#include "type/scalar.hpp"
#include "type/array.hpp"

ArrayType::ArrayType(TypeKind t, std::vector<int64_t> dim) : ScalarType{t}, dim(dim) {}

ArrayType::~ArrayType() {}

std::string ArrayType::getTypeName() {
    std::string s = getTypeKindStr() + " ";
    for (auto &d : dim) {
        s += "[";
        s += std::to_string(d);
        s += "]";
    }
    return s;
}

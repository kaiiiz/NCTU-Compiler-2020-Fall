#include <string>
#include <memory>

#include "type/scalar.hpp"
#include "type/array.hpp"

ArrayType::ArrayType(scalar_type_t t, std::vector<int64_t> dim) : ScalarType{t}, dim(dim) {}

ArrayType::~ArrayType() {}

std::string ArrayType::getTypeName() {
    std::string s = getScalarTypeName() + " ";
    for (auto &d : dim) {
        s += "[";
        s += std::to_string(d);
        s += "]";
    }
    return s;
}

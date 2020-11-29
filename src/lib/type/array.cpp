#include <string>

#include "type/scalar.hpp"
#include "type/array.hpp"

ArrayType::ArrayType(std::string name, std::vector<int64_t> dim) : ScalarType{name}, dim(dim) {}

ArrayType::~ArrayType() {}

std::string ArrayType::getTypeName() {
    std::string s = name + " ";
    for (auto &d : dim) {
        s += "[";
        s += std::to_string(d);
        s += "]";
    }
    return s;
}

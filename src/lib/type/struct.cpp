#include <string>
#include <memory>
#include <iostream>
#include <cassert>

#include "type/struct.hpp"

TypeStruct::TypeStruct(const TypeKind t) : kind(t) {}

TypeStruct::TypeStruct(const TypeKind t, const std::vector<int64_t> dim) : kind(t), dim(dim) {}

std::string TypeStruct::getTypeStr() {
    std::string s = getTypeKindStr(kind);
    if (isArray()) {
        s += " ";
        for (auto &d : dim) {
            s += "[" + std::to_string(d) + "]";
        }
    }
    return s;
}

uint32_t TypeStruct::getDimBytes() {
    assert(kind != TypeKind::String && kind != TypeKind::Void && "Invalid type");
    int bytes = 4;
    if (isArray()) {
        for (auto &d : dim) {
            bytes *= d;
        }
    }
    return bytes;
}

bool TypeStruct::isArray() { return !dim.empty(); }

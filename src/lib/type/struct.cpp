#include <string>
#include <memory>
#include <iostream>

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

uint32_t TypeStruct::getDataSize() {
    switch (kind) {
        case TypeKind::Integer:
            return 4;
        case TypeKind::String:
        case TypeKind::Boolean:
        case TypeKind::Real:
        case TypeKind::Void:
        default:
            break;
    }
    std::cerr << "[TypeStruct] Unimplemented get data size" << std::endl;
    exit(EXIT_FAILURE);
}

bool TypeStruct::isArray() { return !dim.empty(); }

#include <string>

#include "type/struct.hpp"

TypeStruct::TypeStruct(const TypeKind t) : kind(t) {}

TypeStruct::TypeStruct(const TypeKind t, const std::vector<int64_t> dim) : kind(t), dim(dim) {}

std::string TypeStruct::getTypeStr() {
    std::string s = getTypeKindStr();
    if (isArray()) {
        s += " ";
        for (auto &d : dim) {
            s += "[";
            s += std::to_string(d);
            s += "]";
        }
    }
    return s;
}

bool TypeStruct::isArray() { return !dim.empty(); }

std::string TypeStruct::getTypeKindStr() {
    switch (kind) {
        case TypeKind::integer:
            return "integer";
        case TypeKind::string:
            return "string";
        case TypeKind::boolean:
            return "boolean";
        case TypeKind::real:
            return "real";
        case TypeKind::void_:
            return "void";
        default:
            return "<not handled type kind>";
    }
}

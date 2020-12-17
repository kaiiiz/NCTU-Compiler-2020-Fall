#include <string>
#include <memory>

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

bool TypeStruct::isArray() { return !dim.empty(); }

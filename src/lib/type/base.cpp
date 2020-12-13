#include <string>

#include "type/base.hpp"

TypeBase::TypeBase(TypeKind t) : kind(t) {}

TypeBase::~TypeBase() {}

TypeKind TypeBase::getTypeKind() { return kind; }

std::string TypeBase::getTypeKindStr() {
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

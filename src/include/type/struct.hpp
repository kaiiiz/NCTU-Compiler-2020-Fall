#ifndef __TYPE_STRUCT
#define __TYPE_STRUCT

#include <string>
#include <vector>

enum class TypeKind {
    Integer,
    Real,
    String,
    Boolean,
    Void,
};

static inline std::string getTypeKindStr(TypeKind kind) {
    switch (kind) {
        case TypeKind::Integer:
            return "integer";
        case TypeKind::String:
            return "string";
        case TypeKind::Boolean:
            return "boolean";
        case TypeKind::Real:
            return "real";
        case TypeKind::Void:
            return "void";
        default:
            return "<not handled type kind>";
    }
}

class TypeStruct {
   public:
    TypeStruct(const TypeKind t);
    TypeStruct(const TypeKind t, const std::vector<int64_t> dim);
    ~TypeStruct() = default;

    std::string getTypeStr();
    uint32_t getDimBytes();
    bool isArray();

    const TypeKind kind;
    const std::vector<int64_t> dim;
};

#endif
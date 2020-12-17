#ifndef __TYPE_STRUCT
#define __TYPE_STRUCT

#include <string>
#include <vector>

enum class TypeKind {
    integer,
    real,
    string,
    boolean,
    void_,
};

static inline std::string getTypeKindStr(TypeKind kind) {
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

class TypeStruct {
   public:
    TypeStruct(const TypeKind t);
    TypeStruct(const TypeKind t, const std::vector<int64_t> dim);
    ~TypeStruct() = default;

    std::string getTypeStr();
    bool isArray();

    const TypeKind kind;
    const std::vector<int64_t> dim;
};

#endif
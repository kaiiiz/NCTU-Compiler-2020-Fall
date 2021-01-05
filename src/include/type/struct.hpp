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

static inline bool is_void(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Void; }
static inline bool is_real(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Real; }
static inline bool is_int(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Integer; }
static inline bool is_bool(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Boolean; }
static inline bool is_str(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::String; }
static inline std::shared_ptr<TypeStruct> coerce(std::shared_ptr<TypeStruct> t1,
                                                 std::shared_ptr<TypeStruct> t2) {
    // Type coercion is not permitted for array
    if (t1->isArray() || t2->isArray()) return nullptr;

    if (t1->kind == TypeKind::Real && t2->kind == TypeKind::Integer) {
        return t1;
    }
    else if (t2->kind == TypeKind::Real && t1->kind == TypeKind::Integer) {
        return t2;
    }
    return nullptr;
}

#endif
#ifndef __TYPE_BASE
#define __TYPE_BASE

#include <string>
#include <vector>

enum class TypeKind {
    integer,
    real,
    string,
    boolean,
    void_,
};

class TypeStruct {
   public:
    TypeStruct(const TypeKind t);
    TypeStruct(const TypeKind t, const std::vector<int64_t> dim);
    ~TypeStruct() = default;

    std::string getTypeStr();
    bool isArray();

    const TypeKind kind;
    const std::vector<int64_t> dim;

   protected:
    std::string getTypeKindStr();
};

#endif  // __TYPE_BASE
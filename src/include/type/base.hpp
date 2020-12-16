#ifndef __TYPE_BASE
#define __TYPE_BASE

#include <string>

enum class TypeKind {
    integer,
    real,
    string,
    boolean,
    void_,
};

class TypeBase {
   public:
    TypeBase(TypeKind t, const bool is_array);
    ~TypeBase();

    virtual std::string getTypeStr() = 0;
    TypeKind getTypeKind();

   private:
    TypeKind kind;

   public:
    const bool is_array;

   protected:
    std::string getTypeKindStr();
};

#endif  // __TYPE_BASE
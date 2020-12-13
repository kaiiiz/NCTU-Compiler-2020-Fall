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
    TypeBase(TypeKind t);
    ~TypeBase();

    virtual std::string getTypeName() = 0;
    TypeKind getTypeKind();

   private:
    TypeKind kind;

   protected:
    std::string getTypeKindStr();
};

#endif  // __TYPE_BASE
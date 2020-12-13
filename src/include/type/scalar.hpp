#ifndef __TYPE_SCALAR
#define __TYPE_SCALAR

#include <string>

#include "type/base.hpp"

class ScalarType : public TypeBase {
   public:
    ScalarType(TypeKind t);
    ~ScalarType();

    std::string getTypeName() override;

   protected:
    std::string getScalarTypeName();
};

#endif  // __TYPE_SCALAR
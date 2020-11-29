#ifndef __TYPE_SCALAR
#define __TYPE_SCALAR

#include <string>

#include "type/base.hpp"

class ScalarType : public BaseType {
   public:
    ScalarType(std::string name);
    ~ScalarType();

    std::string getTypeName() override;
};

#endif  // __TYPE_SCALAR
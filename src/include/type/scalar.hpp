#ifndef __TYPE_SCALAR
#define __TYPE_SCALAR

#include <string>

#include "type/base.hpp"

enum class scalar_type_t {
    integer,
    real,
    string,
    boolean,
};

class ScalarType : public BaseType {
   public:
    ScalarType(scalar_type_t t);
    ~ScalarType();

    std::string getTypeName() override;
    scalar_type_t getType();

   protected:
    std::string getScalarTypeName();

   private:
    scalar_type_t type;
};

#endif  // __TYPE_SCALAR
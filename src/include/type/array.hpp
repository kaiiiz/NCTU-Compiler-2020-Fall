#ifndef __TYPE_ARRAY
#define __TYPE_ARRAY

#include <string>
#include <vector>
#include <memory>

#include "type/scalar.hpp"

class ArrayType : public ScalarType {
   public:
    ArrayType(scalar_type_t t, std::vector<int64_t> dim);
    ~ArrayType();

    std::string getTypeName() override;

   private:
    std::vector<int64_t> dim;
};

#endif  // __TYPE_ARRAY
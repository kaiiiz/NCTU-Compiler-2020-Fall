#ifndef __TYPE_ARRAY
#define __TYPE_ARRAY

#include <string>
#include <vector>
#include <memory>

#include "type/scalar.hpp"

class ArrayType : public TypeBase {
   public:
    ArrayType(TypeKind t, std::vector<int64_t> dim);
    ~ArrayType();

    std::string getTypeStr() override;
    std::vector<int64_t> getDim();

   private:
    std::vector<int64_t> dim;
};

#endif  // __TYPE_ARRAY
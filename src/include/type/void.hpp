#ifndef __TYPE_VOID
#define __TYPE_VOID

#include <string>

#include "type/base.hpp"

class VoidType : public BaseType {
   public:
    VoidType();
    ~VoidType();

    std::string getTypeName() override;
};

#endif  // __TYPE_VOID
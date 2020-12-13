#ifndef __TYPE_VOID
#define __TYPE_VOID

#include <string>

#include "type/base.hpp"

class VoidType : public TypeBase {
   public:
    VoidType();
    ~VoidType();

    std::string getTypeStr() override;
};

#endif  // __TYPE_VOID
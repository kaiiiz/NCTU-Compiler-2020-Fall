#ifndef __TYPE_BASE
#define __TYPE_BASE

#include <string>

class BaseType {
   public:
    BaseType();
    ~BaseType();

    virtual std::string getTypeName() = 0;
};

#endif  // __TYPE_BASE
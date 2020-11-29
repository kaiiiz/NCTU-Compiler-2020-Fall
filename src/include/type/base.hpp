#ifndef __TYPE_BASE
#define __TYPE_BASE

#include <string>

class BaseType {
   public:
    BaseType(std::string name);
    ~BaseType();

    virtual std::string getTypeName() = 0;

   protected:
    std::string name;
};

#endif  // __TYPE_BASE
#ifndef __TYPE_MANAGER
#define __TYPE_MANAGER

#include <unordered_map>
#include <memory>
#include <vector>

#include "type/struct.hpp"

class TypeManager {
   public:
    TypeManager();

    std::shared_ptr<TypeStruct> getType(TypeKind kind);
    std::shared_ptr<TypeStruct> getType(TypeKind kind, std::vector<int64_t> dim);
    std::shared_ptr<TypeStruct> getType(std::shared_ptr<TypeStruct> type, uint64_t indices_level);

   private:
    std::unordered_map<std::string, std::shared_ptr<TypeStruct>> table;
};

#endif
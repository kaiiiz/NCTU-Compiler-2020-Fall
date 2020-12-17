#include <string>

#include "type/manager.hpp"
#include "type/struct.hpp"

TypeManager type_mgr;

TypeManager::TypeManager() {}

std::shared_ptr<TypeStruct> TypeManager::getType(TypeKind kind) {
    std::string target = getTypeKindStr(kind);
    if (table.find(target) == table.end()) {
        table[target] = std::make_shared<TypeStruct>(kind);
    }
    return table[target];
}

std::shared_ptr<TypeStruct> TypeManager::getType(TypeKind kind, std::vector<int64_t> dim) {
    std::string target = getTypeKindStr(kind);
    for (auto &d : dim) {
        target += "," + std::to_string(d);
    }
    if (table.find(target) == table.end()) {
        table[target] = std::make_shared<TypeStruct>(kind, dim);
    }
    return table[target];
}

#ifndef __FUNCTION_SYMBOL_ENTRY
#define __FUNCTION_SYMBOL_ENTRY

#include <memory>
#include <string>
#include <vector>

#include "sema/SymbolEntry.hpp"

class TypeBase;

class FunctionSymbolEntry : public SymbolEntry {
   public:
    FunctionSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, std::vector<std::shared_ptr<TypeBase>> param_type);

   private:
    std::vector<std::shared_ptr<TypeBase>> param_type;
};

#endif
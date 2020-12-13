#ifndef __LOOP_VAR_SYMBOL_ENTRY
#define __LOOP_VAR_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class TypeBase;

class LoopVarSymbolEntry : public SymbolEntry {
   public:
    LoopVarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type);
};

#endif
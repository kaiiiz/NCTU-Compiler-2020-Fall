#ifndef __LOOP_VAR_SYMBOL_ENTRY
#define __LOOP_VAR_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class TypeStruct;

class LoopVarSymbolEntry : public SymbolEntry {
   public:
    LoopVarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc);

    std::string getAttributeStr() override;
};

#endif
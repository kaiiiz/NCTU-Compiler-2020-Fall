#ifndef __VAR_SYMBOL_ENTRY
#define __VAR_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class VarSymbolEntry : public SymbolEntry {
   public:
    VarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc);

    std::string getAttributeStr() override;
};

#endif
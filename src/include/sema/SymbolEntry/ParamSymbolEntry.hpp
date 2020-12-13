#ifndef __PARAM_SYMBOL_ENTRY
#define __PARAM_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class TypeBase;

class ParamSymbolEntry : public SymbolEntry {
   public:
    ParamSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type);
};

#endif
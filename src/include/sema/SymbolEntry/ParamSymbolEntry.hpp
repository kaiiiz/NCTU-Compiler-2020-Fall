#ifndef __PARAM_SYMBOL_ENTRY
#define __PARAM_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class TypeStruct;

class ParamSymbolEntry : public SymbolEntry {
   public:
    ParamSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc);

    std::string getAttributeStr() override;
};

#endif
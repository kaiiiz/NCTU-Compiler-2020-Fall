#ifndef __PROGRAM_SYMBOL_ENTRY
#define __PROGRAM_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class ProgramSymbolEntry : public SymbolEntry {
   public:
    ProgramSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, const Location &loc);

    std::string getAttributeStr() override;
};

#endif
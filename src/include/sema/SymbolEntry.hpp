#ifndef __SYMBOL_ENTRY
#define __SYMBOL_ENTRY

#include <memory>
#include <string>
#include <vector>

#include "type/base.hpp"
#include "type/scalar.hpp"

enum class SymbolEntryKind {
    program,
    function,
    parameter,
    variable,
    loop_var,
    constant,
};

class SymbolEntry {
   public:
    SymbolEntry(std::string name, SymbolEntryKind kind, uint32_t level, std::shared_ptr<TypeBase> type);

   private:
    std::string name;
    SymbolEntryKind kind;
    uint32_t level;
    std::shared_ptr<TypeBase> type;
};

#endif
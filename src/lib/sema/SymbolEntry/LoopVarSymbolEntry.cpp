#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/LoopVarSymbolEntry.hpp"

LoopVarSymbolEntry::LoopVarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::loop_var, level, type, loc} {}

std::string LoopVarSymbolEntry::getAttributeStr() { return ""; }

#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/LoopVarSymbolEntry.hpp"

LoopVarSymbolEntry::LoopVarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type)
    : SymbolEntry{name, SymbolEntryKind::loop_var, level, type} {}

std::string LoopVarSymbolEntry::getAttributeStr() { return ""; }

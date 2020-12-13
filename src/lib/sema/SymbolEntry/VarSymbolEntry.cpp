#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/VarSymbolEntry.hpp"

VarSymbolEntry::VarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type)
    : SymbolEntry{name, SymbolEntryKind::variable, level, type} {}

std::string VarSymbolEntry::getAttributeStr() { return ""; }

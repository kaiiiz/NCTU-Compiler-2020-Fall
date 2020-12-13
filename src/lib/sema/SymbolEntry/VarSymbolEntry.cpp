#include "sema/SymbolEntry/VarSymbolEntry.hpp"

VarSymbolEntry::VarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type)
    : SymbolEntry{name, SymbolEntryKind::function, level, type} {}

std::string VarSymbolEntry::getAttributeStr() { return ""; }

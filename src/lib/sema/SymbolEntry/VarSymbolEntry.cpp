#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/VarSymbolEntry.hpp"

VarSymbolEntry::VarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::variable, level, type, loc} {}

std::string VarSymbolEntry::getAttributeStr() { return ""; }

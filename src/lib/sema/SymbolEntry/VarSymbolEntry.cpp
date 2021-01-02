#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/VarSymbolEntry.hpp"

VarSymbolEntry::VarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc, const uint32_t fp_offset)
    : SymbolEntry{name, SymbolEntryKind::Variable, level, type, loc}, fp_offset(fp_offset) {}

std::string VarSymbolEntry::getAttributeStr() { return ""; }

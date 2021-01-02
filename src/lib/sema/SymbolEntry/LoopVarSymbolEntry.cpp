#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/LoopVarSymbolEntry.hpp"

LoopVarSymbolEntry::LoopVarSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc, const uint32_t fp_offset)
    : SymbolEntry{name, SymbolEntryKind::LoopVar, level, type, loc}, fp_offset(fp_offset) {}

std::string LoopVarSymbolEntry::getAttributeStr() { return ""; }

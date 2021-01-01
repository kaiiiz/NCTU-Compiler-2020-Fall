#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ProgramSymbolEntry.hpp"

ProgramSymbolEntry::ProgramSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::Program, level, type, loc} {}

std::string ProgramSymbolEntry::getAttributeStr() { return ""; }

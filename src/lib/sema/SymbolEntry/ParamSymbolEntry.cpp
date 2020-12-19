#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ParamSymbolEntry.hpp"

ParamSymbolEntry::ParamSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::Parameter, level, type, loc} {}

std::string ParamSymbolEntry::getAttributeStr() { return ""; }

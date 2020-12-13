#include "sema/SymbolEntry/ParamSymbolEntry.hpp"

ParamSymbolEntry::ParamSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type)
    : SymbolEntry{name, SymbolEntryKind::function, level, type} {}

#include "sema/SymbolEntry/ProgramSymbolEntry.hpp"

ProgramSymbolEntry::ProgramSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type)
    : SymbolEntry{name, SymbolEntryKind::function, level, type} {}

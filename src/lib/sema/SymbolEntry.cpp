#include "type/base.hpp"
#include "sema/SymbolEntry.hpp"

SymbolEntry::SymbolEntry(std::string name, SymbolEntryKind kind, std::uint32_t level, std::shared_ptr<TypeBase> type)
    : name(name), kind(kind), level(level), type(type) {}

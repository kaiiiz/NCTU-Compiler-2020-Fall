#include "sema/SymbolEntry/FunctionSymbolEntry.hpp"

FunctionSymbolEntry::FunctionSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, std::vector<std::shared_ptr<TypeBase>> param_type)
    : SymbolEntry{name, SymbolEntryKind::function, level, type}, param_type(param_type) {}

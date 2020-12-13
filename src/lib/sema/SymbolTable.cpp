#include "sema/SymbolTable.hpp"

SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level)
    : level(level), parent(parent) {}

#include <memory>
#include <stack>

#include "sema/SymbolManager.hpp"
#include "sema/SymbolTable.hpp"

SymbolManager::SymbolManager() {
    global = std::make_shared<SymbolTable>(nullptr, 0);
    tables.push(global);
}

std::shared_ptr<SymbolTable> SymbolManager::currentSymTab() { return tables.top(); }

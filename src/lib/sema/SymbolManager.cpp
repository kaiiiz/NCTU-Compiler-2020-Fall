#include <memory>
#include <stack>

#include "sema/SymbolManager.hpp"
#include "sema/SymbolTable.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

SymbolManager::SymbolManager() {
    global = std::make_shared<SymbolTable>(nullptr, 0, ContextKind::Program, type_mgr.getType(TypeKind::Void));
    tables.push(global);
}

void SymbolManager::pushScope(std::shared_ptr<SymbolTable> new_scope) {
    tables.push(new_scope);
}

void SymbolManager::popScope() {
    tables.pop();
}

std::shared_ptr<SymbolTable> SymbolManager::currentSymTab() { return tables.top(); }

void SymbolManager::dumpSymTab() {
    global->dump();
}

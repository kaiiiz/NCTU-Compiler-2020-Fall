#include <memory>
#include <cassert>

#include "AST/base/AstNodeWithSymTab.hpp"
#include "sema/SymbolTable.hpp"

std::shared_ptr<SymbolTable> AstNodeWithSymTab::getSymTab() {
    assert(sym_tab != nullptr && "Symbol table doesn't exist");
    return sym_tab;
};

void AstNodeWithSymTab::fillSymTab(std::shared_ptr<SymbolTable> tab) { sym_tab = tab; };

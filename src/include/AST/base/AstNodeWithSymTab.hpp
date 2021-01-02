#ifndef __AST_NODE_WITH_SYM_TAB_BASE_H
#define __AST_NODE_WITH_SYM_TAB_BASE_H

#include <memory>

class SymbolTable;

class AstNodeWithSymTab {
  public:
    std::shared_ptr<SymbolTable> getSymTab();
    void fillSymTab(std::shared_ptr<SymbolTable> sym_tab);

  protected:
    std::shared_ptr<SymbolTable> sym_tab;
};

#endif

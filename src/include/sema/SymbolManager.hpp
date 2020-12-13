#ifndef __SYMBOL_MANAGER
#define __SYMBOL_MANAGER

#include <memory>
#include <stack>

class SymbolTable;

class SymbolManager {
   public:
    SymbolManager();

    void pushScope(std::shared_ptr<SymbolTable> new_scope);
    void popScope();
    std::shared_ptr<SymbolTable> currentSymTab();
    void dumpSymTab();

   private:
    std::shared_ptr<SymbolTable> global;
    std::stack<std::shared_ptr<SymbolTable>> tables;
};

#endif
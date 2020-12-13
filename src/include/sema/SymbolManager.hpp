#ifndef __SYMBOL_MANAGER
#define __SYMBOL_MANAGER

#include <memory>
#include <stack>

class SymbolTable;

class SymbolManager {
   public:
    SymbolManager();

    void pushScope(std::shared_ptr<SymbolTable> *new_scope);
    void popScope();
    // other methods
   private:
    std::stack<std::shared_ptr<SymbolTable>> tables;
};

#endif
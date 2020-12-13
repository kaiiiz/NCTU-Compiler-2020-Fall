#ifndef __SYMBOL_TABLE
#define __SYMBOL_TABLE

#include <vector>
#include <memory>

class SymbolEntry;

class SymbolTable {
   public:
    SymbolTable();

    void addSymbol(/* attributes needed by a SymbolEntry */);
    // other methods
   private:
    std::vector<std::shared_ptr<SymbolEntry>> entries;
};

#endif
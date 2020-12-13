#ifndef __SYMBOL_TABLE
#define __SYMBOL_TABLE

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "sema/SymbolEntry.hpp"

class SymbolTable {
   public:
    SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level);

    void addSymbol(/* attributes needed by a SymbolEntry */);

    const std::uint32_t level;

   private:
    std::shared_ptr<SymbolTable> parent;
    std::vector<std::string> entries; // order list
    std::unordered_map<std::string, std::shared_ptr<SymbolTable>> childs;
    std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> entries_instance;
};

#endif
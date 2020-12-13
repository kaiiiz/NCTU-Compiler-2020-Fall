#ifndef __SYMBOL_TABLE
#define __SYMBOL_TABLE

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class SymbolEntry;

class SymbolTable {
   public:
    SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level);

    void insert(std::shared_ptr<SymbolEntry> symbol);
    std::shared_ptr<SymbolEntry> lookup(std::string name);
    void addChild(std::shared_ptr<SymbolTable>);

    void dump();

    const std::uint32_t level;

   private:
    void dumpDemarcation(const char chr);

    std::shared_ptr<SymbolTable> parent;
    std::vector<std::shared_ptr<SymbolTable>> childs;
    std::vector<std::string> entries; // order list
    std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> entries_instance;
};

#endif
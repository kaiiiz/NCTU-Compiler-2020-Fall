#ifndef __SYMBOL_MANAGER
#define __SYMBOL_MANAGER

#include <memory>
#include <stack>
#include <unordered_map>

class SymbolTable;

class SymbolManager {
   public:
    SymbolManager();

    void pushScope(std::shared_ptr<SymbolTable> new_scope);
    void popScope();
    std::shared_ptr<SymbolTable> currentSymTab();
    void dumpSymTab();

    void addRoData(float rval);
    void addRoData(std::string sval);
    std::string getRoDataIdentifier(float rval);
    std::string getRoDataIdentifier(std::string sval);
    std::unordered_map<std::string, std::string> getRealRoData();
    std::unordered_map<std::string, std::string> getStrRoData();

   private:
    std::shared_ptr<SymbolTable> global;
    std::stack<std::shared_ptr<SymbolTable>> tables;
    int rodata_real_idx = 0;
    int rodata_str_idx = 0;
    std::unordered_map<std::string, std::string> rodata_real; // map to its id in assembly
    std::unordered_map<std::string, std::string> rodata_string; // map to its id in assembly
};

#endif
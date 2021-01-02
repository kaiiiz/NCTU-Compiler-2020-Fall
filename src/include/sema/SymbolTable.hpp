#ifndef __SYMBOL_TABLE
#define __SYMBOL_TABLE

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "type/struct.hpp"

class SymbolEntry;

enum class ContextKind {
    Program,
    Function,
    Procedure,
};

class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
   public:
    SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level, const ContextKind ctx_kind, const std::shared_ptr<TypeStruct> ctx_type);

    void insert(std::shared_ptr<SymbolEntry> symbol);
    std::shared_ptr<SymbolEntry> lookup(std::string name);
    void addChild(std::shared_ptr<SymbolTable>);
    uint32_t getFpOffset();

    void dump();

    const std::uint32_t level;
    const ContextKind ctx_kind;
    const std::shared_ptr<TypeStruct> ctx_type;

   private:
    void dumpDemarcation(const char chr);

    uint32_t fp_offset = 8; // 0-3: pc, 4-7: fp
    std::shared_ptr<SymbolTable> parent;
    std::vector<std::shared_ptr<SymbolTable>> childs;
    std::vector<std::string> entries; // order list
    std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> entries_instance;
};

#endif
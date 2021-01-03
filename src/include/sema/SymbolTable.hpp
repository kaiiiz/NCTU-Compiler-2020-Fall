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

class SymbolFPManager {
   public:
    SymbolFPManager();

    uint32_t getNextFpOffset();

   private:
    uint32_t fp_offset = 20; // 0-3: pc, 4-7: fp, 8-11: t0, 12-15: t1, 16-19: t2
};

class SymbolTable : public std::enable_shared_from_this<SymbolTable> {
   public:
    SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level, const ContextKind ctx_kind, const std::shared_ptr<TypeStruct> ctx_type, const std::shared_ptr<SymbolFPManager> fp_mgr);

    void insert(std::shared_ptr<SymbolEntry> symbol);
    std::shared_ptr<SymbolEntry> lookup(std::string name);
    void addChild(std::shared_ptr<SymbolTable>);

    void dump();

    const std::uint32_t level;
    const ContextKind ctx_kind;
    const std::shared_ptr<TypeStruct> ctx_type;
    const std::shared_ptr<SymbolFPManager> fp_mgr;

   private:
    void dumpDemarcation(const char chr);

    std::shared_ptr<SymbolTable> parent;
    std::vector<std::shared_ptr<SymbolTable>> childs;
    std::vector<std::string> entries; // order list
    std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> entries_instance;
};

#endif
#ifndef __SYMBOL_ENTRY
#define __SYMBOL_ENTRY

#include <memory>
#include <string>
#include <vector>

#include "AST/ast.hpp"
#include "type/struct.hpp"

enum class SymbolEntryKind {
    Program,
    Function,
    Parameter,
    Variable,
    LoopVar,
    Constant,
};

class SymbolEntry {
   public:
    SymbolEntry(std::string name, SymbolEntryKind kind, uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc);

    std::string getNameStr();
    std::string getKindStr();
    SymbolEntryKind getKind();
    uint32_t getLevel();
    std::shared_ptr<TypeStruct> getType();
    std::string getTypeStr();
    virtual std::string getAttributeStr() = 0;
    const Location &getLocation() const;

   protected:
    const Location &location;
    std::string name;
    SymbolEntryKind kind;
    uint32_t level;
    std::shared_ptr<TypeStruct> type;
};

#endif
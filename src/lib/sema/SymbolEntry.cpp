#include <memory>
#include <string>

#include "type/struct.hpp"
#include "sema/SymbolEntry.hpp"

SymbolEntry::SymbolEntry(std::string symbol_name, SymbolEntryKind kind, std::uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc)
    : location(loc), kind(kind), level(level), type(type) {
    if (symbol_name.length() > 32)
        symbol_name.erase(32, std::string::npos);
    name = symbol_name;
}

std::string SymbolEntry::getNameStr() { return name; }

std::string SymbolEntry::getKindStr() {
    switch (kind) {
        case SymbolEntryKind::constant:
            return "constant";
        case SymbolEntryKind::function:
            return "function";
        case SymbolEntryKind::loop_var:
            return "loop_var";
        case SymbolEntryKind::parameter:
            return "parameter";
        case SymbolEntryKind::program:
            return "program";
        case SymbolEntryKind::variable:
            return "variable";
        default:
            return "<not handled symbol entry>";
    }
}

SymbolEntryKind SymbolEntry::getKind() { return kind; }

uint32_t SymbolEntry::getLevel() { return level; }

std::shared_ptr<TypeStruct> SymbolEntry::getType() { return type; }

std::string SymbolEntry::getTypeStr() { return type->getTypeStr(); }

const Location &SymbolEntry::getLocation() const { return location; }

#include <memory>

#include "type/struct.hpp"
#include "sema/SymbolEntry.hpp"

SymbolEntry::SymbolEntry(std::string name, SymbolEntryKind kind, std::uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc)
    : location(loc), name(name), kind(kind), level(level), type(type) {}

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

int SymbolEntry::getLevel() { return level; }

std::shared_ptr<TypeStruct> SymbolEntry::getType() { return type; }

std::string SymbolEntry::getTypeStr() { return type->getTypeStr(); }

#include <memory>
#include <string>
#include <iostream>
#include <cassert>

#include "type/struct.hpp"
#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ConstSymbolEntry.hpp"
#include "sema/SymbolEntry/FunctionSymbolEntry.hpp"
#include "sema/SymbolEntry/LoopVarSymbolEntry.hpp"
#include "sema/SymbolEntry/ParamSymbolEntry.hpp"
#include "sema/SymbolEntry/ProgramSymbolEntry.hpp"
#include "sema/SymbolEntry/VarSymbolEntry.hpp"

SymbolEntry::SymbolEntry(std::string symbol_name, SymbolEntryKind kind, std::uint32_t level, std::shared_ptr<TypeStruct> type, const Location &loc)
    : location(loc), kind(kind), level(level), type(type) {
    if (symbol_name.length() > 32)
        symbol_name.erase(32, std::string::npos);
    name = symbol_name;
}

std::string SymbolEntry::getNameStr() { return name; }

std::string SymbolEntry::getKindStr() {
    switch (kind) {
        case SymbolEntryKind::Constant:
            return "constant";
        case SymbolEntryKind::Function:
            return "function";
        case SymbolEntryKind::LoopVar:
            return "loop_var";
        case SymbolEntryKind::Parameter:
            return "parameter";
        case SymbolEntryKind::Program:
            return "program";
        case SymbolEntryKind::Variable:
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

uint32_t SymbolEntry::getFpOffset() {
    assert(level != 0 && "Cannot get fp offset from global symbol");

    switch (kind) {
        case SymbolEntryKind::Parameter:
            return std::dynamic_pointer_cast<ParamSymbolEntry>(shared_from_this())->fp_offset;
        case SymbolEntryKind::Constant:
            return std::dynamic_pointer_cast<LocalConstSymbolEntry>(shared_from_this())->fp_offset;
        case SymbolEntryKind::LoopVar:
            return std::dynamic_pointer_cast<LoopVarSymbolEntry>(shared_from_this())->fp_offset;
        case SymbolEntryKind::Variable:
            return std::dynamic_pointer_cast<LocalVarSymbolEntry>(shared_from_this())->fp_offset;
        default:
            assert(false && "Cannot get fp offset from non variable symbol");
    }
}

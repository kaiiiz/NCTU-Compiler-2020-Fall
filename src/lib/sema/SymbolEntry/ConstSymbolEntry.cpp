#include <string>

#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ConstSymbolEntry.hpp"
#include "type/struct.hpp"

ConstSymbolEntry::ConstSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeStruct> type, std::string value_str, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::Constant, level, type, loc}, value_str(value_str) {}

std::string ConstSymbolEntry::getAttributeStr() {
    if (type->kind == TypeKind::Boolean) {
        if (std::stoi(value_str)) {
            return "true";
        }
        else {
            return "false";
        }
    }
    return value_str;

}

LocalConstSymbolEntry::LocalConstSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeStruct> type, std::string value_str, const Location &loc, const uint32_t fp_offset)
    : ConstSymbolEntry{name, level, type, value_str, loc}, fp_offset(fp_offset) {}

#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ConstSymbolEntry.hpp"
#include "type/struct.hpp"

ConstIntSymbolEntry::ConstIntSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeStruct> type, int64_t ival, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type, loc}, ival(ival) {}

std::string ConstIntSymbolEntry::getAttributeStr() {
    return std::to_string(ival);
}

ConstBoolSymbolEntry::ConstBoolSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeStruct> type, bool bval, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type, loc}, bval(bval) {}

std::string ConstBoolSymbolEntry::getAttributeStr() {
    return (bval) ? "true" : "false";
}

ConstRealSymbolEntry::ConstRealSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeStruct> type, double rval, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type, loc}, rval(rval) {}

std::string ConstRealSymbolEntry::getAttributeStr() {
    return std::to_string(rval);
}

ConstStrSymbolEntry::ConstStrSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeStruct> type, std::string sval, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type, loc}, sval(sval) {}

std::string ConstStrSymbolEntry::getAttributeStr() {
    return sval;
}

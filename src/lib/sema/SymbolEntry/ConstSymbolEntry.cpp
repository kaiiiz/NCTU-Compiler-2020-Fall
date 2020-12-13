#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ConstSymbolEntry.hpp"
#include "type/scalar.hpp"

ConstIntSymbolEntry::ConstIntSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeBase> type, int64_t ival)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, ival(ival) {}

std::string ConstIntSymbolEntry::getAttributeStr() {
    return std::to_string(ival);
}

ConstBoolSymbolEntry::ConstBoolSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeBase> type, bool bval)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, bval(bval) {}

std::string ConstBoolSymbolEntry::getAttributeStr() {
    return (bval) ? "true" : "false";
}

ConstRealSymbolEntry::ConstRealSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeBase> type, double rval)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, rval(rval) {}

std::string ConstRealSymbolEntry::getAttributeStr() {
    return std::to_string(rval);
}

ConstStrSymbolEntry::ConstStrSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<TypeBase> type, std::string sval)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, sval(sval) {}

std::string ConstStrSymbolEntry::getAttributeStr() {
    return sval;
}

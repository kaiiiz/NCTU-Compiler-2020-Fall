#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ConstSymbolEntry.hpp"
#include "type/scalar.hpp"

ConstIntSymbolEntry::ConstIntSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<ScalarType> type, int64_t ival)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, ival(ival) {}

ConstBoolSymbolEntry::ConstBoolSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<ScalarType> type, bool bval)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, bval(bval) {}

ConstRealSymbolEntry::ConstRealSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<ScalarType> type, double rval)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, rval(rval) {}

ConstStrSymbolEntry::ConstStrSymbolEntry(std::string name, std::uint32_t level, std::shared_ptr<ScalarType> type, std::string sval)
    : SymbolEntry{name, SymbolEntryKind::constant, level, type}, sval(sval) {}

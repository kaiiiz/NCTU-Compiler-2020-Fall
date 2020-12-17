#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/FunctionSymbolEntry.hpp"

FunctionSymbolEntry::FunctionSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, std::vector<std::shared_ptr<TypeStruct>> param_type, const Location &loc)
    : SymbolEntry{name, SymbolEntryKind::function, level, type, loc}, param_type(param_type) {}

std::vector<std::shared_ptr<TypeStruct>> FunctionSymbolEntry::getParamType() { return param_type; }

std::string FunctionSymbolEntry::getAttributeStr() {
    std::string s;
    for (auto &p : param_type) {
        s += p->getTypeStr();
        if (&p != &param_type.back()) s += ", ";
    }
    return s;
}

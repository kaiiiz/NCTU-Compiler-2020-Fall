#ifndef __CONST_SYMBOL_ENTRY
#define __CONST_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class TypeStruct;

class ConstSymbolEntry : public SymbolEntry {
   public:
    ConstSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const std::string value_str, const Location &loc);

    std::string getAttributeStr() override;

    const std::string value_str;
};

class LocalConstSymbolEntry : public ConstSymbolEntry {
   public:
    LocalConstSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, const std::string value_str, const Location &loc, const uint32_t fp_offset);

    const uint32_t fp_offset;
};

#endif
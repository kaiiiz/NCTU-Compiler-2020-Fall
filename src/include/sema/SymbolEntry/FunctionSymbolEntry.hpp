#ifndef __FUNCTION_SYMBOL_ENTRY
#define __FUNCTION_SYMBOL_ENTRY

#include <memory>
#include <string>
#include <vector>

#include "sema/SymbolEntry.hpp"

class TypeStruct;

class FunctionSymbolEntry : public SymbolEntry {
   public:
    FunctionSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeStruct> type, std::vector<std::shared_ptr<TypeStruct>> param_type, const Location &loc);

    std::string getAttributeStr() override;
    std::vector<std::shared_ptr<TypeStruct>> getParamType();

   private:
    std::vector<std::shared_ptr<TypeStruct>> param_type;
};

#endif
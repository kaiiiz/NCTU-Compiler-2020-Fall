#ifndef __CONST_SYMBOL_ENTRY
#define __CONST_SYMBOL_ENTRY

#include <memory>
#include <string>

#include "sema/SymbolEntry.hpp"

class TypeBase;

class ConstIntSymbolEntry : public SymbolEntry {
   public:
    ConstIntSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, int64_t ival, const Location &loc);

    std::string getAttributeStr() override;

   private:
    int64_t ival;
};

class ConstBoolSymbolEntry : public SymbolEntry {
   public:
    ConstBoolSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, bool bval, const Location &loc);

    std::string getAttributeStr() override;

   private:
    bool bval;
};

class ConstRealSymbolEntry : public SymbolEntry {
   public:
    ConstRealSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, double rval, const Location &loc);

    std::string getAttributeStr() override;

   private:
    double rval;
};

class ConstStrSymbolEntry : public SymbolEntry {
   public:
    ConstStrSymbolEntry(std::string name, uint32_t level, std::shared_ptr<TypeBase> type, std::string sval, const Location &loc);

    std::string getAttributeStr() override;

   private:
    std::string sval;
};


#endif
#include "sema/SymbolTable.hpp"

SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level)
    : level(level), parent(parent) {}

void SymbolTable::dumpDemarcation(const char chr) {
    for (size_t i = 0; i < 110; ++i) {
        printf("%c", chr);
    }
    puts("");
}

void SymbolTable::dump() {
    dumpDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type", "Attribute");
    dumpDemarcation('-');
    {
        printf("%-33s", "func");
        printf("%-11s", "function");
        printf("%d%-10s", 0, "(global)");
        printf("%-17s", "boolean");
        printf("%-11s", "integer, real [2][3]");
        puts("");
    }
    dumpDemarcation('-');
}

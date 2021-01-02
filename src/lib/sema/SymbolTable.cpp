#include "sema/SymbolTable.hpp"

#include <iostream>
#include <memory>

#include "sema/SymbolEntry.hpp"

SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent, const std::uint32_t level, const ContextKind ctx_kind, const std::shared_ptr<TypeStruct> ctx_type)
    : level(level), ctx_kind(ctx_kind), ctx_type(ctx_type), parent(parent) {}

void SymbolTable::addChild(std::shared_ptr<SymbolTable> child) {
    childs.push_back(child);
}

void SymbolTable::insert(std::shared_ptr<SymbolEntry> symbol) {
    std::string symbol_name = symbol->getNameStr();
    entries.push_back(symbol_name);
    entries_instance[symbol_name] = symbol;
}

std::shared_ptr<SymbolEntry> SymbolTable::lookup(std::string name) {
    auto table = shared_from_this();
    while (table != nullptr) {
        auto find = table->entries_instance.find(name);
        if (find != entries_instance.end()) return find->second;
        table = table->parent;
    }
    return nullptr;
}

uint32_t SymbolTable::getFpOffset(uint32_t data_size) {
    fp_offset += data_size;
    return fp_offset;
}

void SymbolTable::dumpDemarcation(const char chr) {
    for (size_t i = 0; i < 110; ++i) {
        printf("%c", chr);
    }
    puts("");
}

void SymbolTable::dump() {
    for (auto &c : childs) {
        c->dump();
    }

    dumpDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type", "Attribute");
    dumpDemarcation('-');
    {
        for (auto &e : entries) {
            auto e_inst = entries_instance[e];
            printf("%-33s", e_inst->getNameStr().c_str());
            printf("%-11s", e_inst->getKindStr().c_str());
            if (e_inst->getLevel() == 0) {
                printf("%d%-10s", 0, "(global)");
            }
            else {
                printf("%d%-10s", e_inst->getLevel(), "(local)");
            }
            printf("%-17s", e_inst->getTypeStr().c_str());
            printf("%-11s", e_inst->getAttributeStr().c_str());
            puts("");
        }
    }
    dumpDemarcation('-');
}

#include <memory>
#include <stack>
#include <iostream>

#include "sema/SymbolManager.hpp"
#include "sema/SymbolTable.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

SymbolManager::SymbolManager() {
    auto fp_mgr = std::make_shared<SymbolFPManager>();
    global = std::make_shared<SymbolTable>(nullptr, 0, ContextKind::Program, type_mgr.getType(TypeKind::Void), fp_mgr);
    tables.push(global);
}

void SymbolManager::pushScope(std::shared_ptr<SymbolTable> new_scope) {
    tables.push(new_scope);
}

void SymbolManager::popScope() {
    tables.pop();
}

std::shared_ptr<SymbolTable> SymbolManager::currentSymTab() { return tables.top(); }

void SymbolManager::dumpSymTab() {
    global->dump();
    for (auto it = rodata_real.begin(); it != rodata_real.end(); it++) {
        std::cout << it->first << " " << it->second << "\n";
    }
    for (auto it = rodata_string.begin(); it != rodata_string.end(); it++) {
        std::cout << it->first << " " << it->second << "\n";
    }
}

void SymbolManager::addRoData(float rval) {
    rodata_real[std::to_string(rval)] = ".r" + std::to_string(rodata_real_idx++);
}

void SymbolManager::addRoData(std::string sval) {
    rodata_string[sval] = ".s" + std::to_string(rodata_str_idx++);
}

std::string SymbolManager::getRoDataIdentifier(float rval) {
    auto find = rodata_real.find(std::to_string(rval));
    if (find != rodata_real.end()) return find->second;
    return nullptr;
}

std::string SymbolManager::getRoDataIdentifier(std::string sval) {
    auto find = rodata_string.find(sval);
    if (find != rodata_string.end()) return find->second;
    return nullptr;
}

std::unordered_map<std::string, std::string> SymbolManager::getRealRoData() {
    return rodata_real;
}

std::unordered_map<std::string, std::string> SymbolManager::getStrRoData() {
    return rodata_string;
}

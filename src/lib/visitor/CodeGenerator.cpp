#include "visitor/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdarg>
#include <cstdio>

CodeGenerator::CodeGenerator(SymbolManager &symbol_mgr, const std::string &in_file_name, const std::string &out_file_name) 
    : in_file_name(in_file_name), symbol_mgr(symbol_mgr) {
    output_file.open(out_file_name, std::ofstream::trunc | std::ofstream::out);
    if (!output_file.good()) {
        std::cerr << "[CodeGenerator] File stream is not good" << std::endl;
        exit(EXIT_FAILURE);
    }
}

CodeGenerator::~CodeGenerator() {
    output_file.close();
}

void CodeGenerator::visit(ProgramNode &p_program) {
    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    // symbol_manager->reconstructHashTableFromSymbolTable(p_program.getSymbolTable());

    // // Generate RISC-V instructions for program header
    // dumpInstrs("%s%s%s%s", 
    //     "   .file ", this->in_file_name, "\n",
    //     "   .option nopic\n"
    // );

    // p_program.visitChildNodes(*this);

    // // Remove the entries in the hash table
    // symbol_manager->removeSymbolsFromHashTable(p_program.getSymbolTable());
}

void CodeGenerator::visit(DeclNode &p_decl) {

}

void CodeGenerator::visit(VariableNode &p_variable) {

}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {

}

void CodeGenerator::visit(FunctionNode &p_function) {
    // // Reconstruct the hash table for looking up the symbol entry
    // symbol_manager->reconstructHashTableFromSymbolTable(p_function.getSymbolTable());

    // // Remove the entries in the hash table
    // symbol_manager->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // // Reconstruct the hash table for looking up the symbol entry
    // symbol_manager->reconstructHashTableFromSymbolTable(p_compound_statement.getSymbolTable());

    // // Remove the entries in the hash table
    // symbol_manager->removeSymbolsFromHashTable(p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {

}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {

}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {

}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {

}

void CodeGenerator::visit(FunctionCallNode &p_func_call) {

}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {

}

void CodeGenerator::visit(AssignmentNode &p_assignment) {

}

void CodeGenerator::visit(ReadNode &p_read) {

}

void CodeGenerator::visit(IfNode &p_if) {

}

void CodeGenerator::visit(WhileNode &p_while) {

}

void CodeGenerator::visit(ForNode &p_for) {
    // // Reconstruct the hash table for looking up the symbol entry
    // symbol_manager->reconstructHashTableFromSymbolTable(p_for.getSymbolTable());

    // // Remove the entries in the hash table
    // symbol_manager->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {

} 

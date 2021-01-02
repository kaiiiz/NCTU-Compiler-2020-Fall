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
    output_file << "    .file \"" << in_file_name << "\"\n"
                << "    .option nopic\n";

    p_program.visitChildNodes(*this);
}

void CodeGenerator::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void CodeGenerator::visit(VariableNode &p_variable) {
    auto symTab = symbol_mgr.currentSymTab();
    auto varName = p_variable.getNameStr();
    auto symbol = symTab->lookup(varName);
    if (symbol->getLevel() == 0) { // global var decl
        if (symbol->getKind() == SymbolEntryKind::Constant) { // .rodata
            auto constSymbol = std::dynamic_pointer_cast<ConstSymbolEntry>(symbol);
            genGlobalVarConst(varName, constSymbol->value_str);
        } else { // .bss
            genGlobalVarDecl(varName, 4, 4);
        }
    } else {
        if (symbol->getKind() == SymbolEntryKind::Constant) { // local var const
            genLocalVarAddrStore(symbol->getFpOffset());
            p_variable.visitChildNodes(*this); // push constant value to stack
            genAssign();
        }
    }
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    genConstStore(p_constant_value.value_str);
}

void CodeGenerator::visit(FunctionNode &p_function) {
    // // Reconstruct the hash table for looking up the symbol entry
    // symbol_manager->reconstructHashTableFromSymbolTable(p_function.getSymbolTable());

    // // Remove the entries in the hash table
    // symbol_manager->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    if (p_compound_statement.getKind() != CompoundKind::Function) {
        symbol_mgr.pushScope(p_compound_statement.getSymTab());
    }

    if (p_compound_statement.getKind() == CompoundKind::Main) { // main
        genFunctionPrologue("main");
    }
    p_compound_statement.visitChildNodes(*this);
    if (p_compound_statement.getKind() == CompoundKind::Main) { // main
        genFunctionEpilogue("main");
    }

    if (p_compound_statement.getKind() != CompoundKind::Function) {
        symbol_mgr.popScope();
    }
}

void CodeGenerator::visit(PrintNode &p_print) {
    p_print.visitChildNodes(*this);
    genPrint();
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);
    genBinaryOperation(p_bin_op.op);
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    p_un_op.visitChildNodes(*this);
    genUnaryOperation(p_un_op.op);
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {

}

void CodeGenerator::visit(FunctionCallNode &p_func_call) {

}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    auto symTab = symbol_mgr.currentSymTab();
    auto varName = p_variable_ref.name;
    auto symbol = symTab->lookup(varName);

    switch (p_variable_ref.getSide()) {
        case Side::LHS:
            if (symbol->getLevel() == 0) { // global var ref
                genGlobalVarAddrStore(varName);
            } else { // local var ref
                genLocalVarAddrStore(symbol->getFpOffset());
            }
            break;
        case Side::RHS:
            if (symbol->getLevel() == 0) { // global var ref
                genGlobalVarLoad(varName);
            } else { // local var ref
                genLocalVarLoad(symbol->getFpOffset());
            }
            break;
    }
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    p_assignment.visitChildNodes(*this);
    genAssign();
}

void CodeGenerator::visit(ReadNode &p_read) {
    p_read.visitChildNodes(*this);
    genRead();
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

void CodeGenerator::genFunctionPrologue(std::string func_name) {
    output_file << ".section    .text\n"
                << "    .align 2\n"
                << "    .globl " << func_name << "\n"
                << "    .type " << func_name << ", @function\n"
                << func_name << ":\n"
                << "    # in the function prologue\n"
                << "    addi sp, sp, -128\n"
                << "    sw ra, 124(sp)\n"
                << "    sw s0, 120(sp)\n"
                << "    addi s0, sp, 128\n"
                << "    # in the function body\n";
}

void CodeGenerator::genFunctionEpilogue(std::string func_name) {
    output_file << "    # in the function epilogue\n"
                << "    lw ra, 124(sp)\n"
                << "    lw s0, 120(sp)\n"
                << "    addi sp, sp, 128\n"
                << "    jr ra\n"
                << "    .size " << func_name << ", .-" << func_name << "\n";
}

void CodeGenerator::genGlobalVarDecl(std::string var_name, int size, int align) {
    output_file << "    .comm " << var_name << ", " << size << ", " << align << "\n";
}

void CodeGenerator::genGlobalVarAddrStore(std::string var_name) {
    output_file << "    # global var addr store\n"
                << "    la t0, " << var_name << "\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genLocalVarAddrStore(uint32_t fp_offset) {
    output_file << "    # local var addr store\n"
                << "    addi t0, s0, -" << fp_offset << "\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genConstStore(std::string val) {
    // TODO: for non integer constant
    output_file << "    # const int store\n"
                << "    li t0, " << val << "\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genAssign() {
    output_file << "    # var assign\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    lw t1, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    sw t0, 0(t1)\n";
}

void CodeGenerator::genGlobalVarConst(std::string var_name, std::string val_str) {
    // TODO: for non integer constant
    output_file << ".section    .rodata\n"
                << "    .align 2\n"
                << "    .globl " << var_name << "\n"
                << "    .type " << var_name << ", @object\n"
                << var_name << ":\n"
                << "    .word " << val_str << "\n";
}

void CodeGenerator::genGlobalVarLoad(std::string var_name) {
    output_file << "    # global var load\n"
                << "    la t0, " << var_name << "\n"
                << "    lw t1, 0(t0)\n"
                << "    mv t0, t1\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genLocalVarLoad(uint32_t fp_offset) {
    output_file << "    # local var load\n"
                << "    lw t0, -" << fp_offset << "(s0)\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genPrint() {
    output_file << "    # print\n"
                << "    lw a0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    jal ra, printInt\n";
}

void CodeGenerator::genRead() {
    output_file << "    # read\n"
                << "    jal ra, readInt\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    sw a0, 0(t0)\n";
}

void CodeGenerator::genBinaryOperation(BinaryOP op) {
    output_file << "    # binary operation\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    lw t1, 0(sp)\n"
                << "    addi sp, sp, 4\n";

    switch (op) {
        case BinaryOP::PLUS:
            output_file << "    add t0, t1, t0\n";
            break;
        case BinaryOP::MINUS:
            output_file << "    sub t0, t1, t0\n";
            break;
        case BinaryOP::MULTIPLY:
            output_file << "    mul t0, t1, t0\n";
            break;
        case BinaryOP::DIVIDE:
            output_file << "    div t0, t1, t0\n";
            break;
        case BinaryOP::MOD:
            output_file << "    rem t0, t1, t0\n";
            break;
        case BinaryOP::AND:
            output_file << "    and t0, t1, t0\n";
            break;
        case BinaryOP::OR:
            output_file << "    or t0, t1, t0\n";
            break;
        case BinaryOP::LESS:
            output_file << "    slt t0, t1, t0\n"
	                    << "    andi t0, t0, 0xff\n";
            break;
        case BinaryOP::LESS_OR_EQUAL:
            output_file << "    sgt t0, t1, t0\n"
                        << "    xori t0, t0, 1\n"
	                    << "    andi t0, t0, 0xff\n";
            break;
        case BinaryOP::GREATER:
            output_file << "    sgt t0, t1, t0\n"
	                    << "    andi t0, t0, 0xff\n";
            break;
        case BinaryOP::GREATER_OR_EQUAL:
            output_file << "    slt t0, t1, t0\n"
                        << "    xori t0, t0, 1\n"
	                    << "    andi t0, t0, 0xff\n";
            break;
        case BinaryOP::EQUAL:
            output_file << "    sub t0, t1, t0\n"
                        << "    seqz t0, t0\n"
	                    << "    andi t0, t0, 0xff\n";
            break;
        case BinaryOP::NOT_EQUAL:
            output_file << "    sub t0, t1, t0\n"
                        << "    snez t0, t0\n"
	                    << "    andi t0, t0, 0xff\n";
            break;
    }

    output_file << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genUnaryOperation(UnaryOP op) {
    std::string unary_op_str;
    switch (op) {
        case UnaryOP::MINUS:
            unary_op_str = "neg";
            break;
        default:
            std::cerr << "[CodeGenerator] Unimplemented unary operation" << std::endl;
            exit(EXIT_FAILURE);
    }

    output_file << "    # unary operation\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    " << unary_op_str << " t0, t0\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}


#include "visitor/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdarg>
#include <cstdio>

#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

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

    auto rodata_real = symbol_mgr.getRealRoData();
    for (auto it = rodata_real.begin(); it != rodata_real.end(); it++) {
        genRoData(std::stof(it->first), it->second);
    }

    auto rodata_str = symbol_mgr.getStrRoData();
    for (auto it = rodata_str.begin(); it != rodata_str.end(); it++) {
        genRoData(it->first, it->second);
    }

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
            genGlobalVarDecl(varName, symbol->getType()->getDimBytes(), 4);
        }
    } else {
        if (symbol->getKind() == SymbolEntryKind::Constant) { // local var const
            genLocalVarAddrLoad(symbol->getFpOffset(),
                                symbol->getKind(),
                                symbol->getType(),
                                std::vector<std::shared_ptr<ExpressionBase>>());
            p_variable.visitChildNodes(*this); // push constant value to stack
            genAssign(symbol->getType());
        }
    }
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    switch (p_constant_value.getType()->kind) {
        case TypeKind::Integer:
        case TypeKind::Boolean:
            genConstStore(std::stoi(p_constant_value.value_str));
            break;
        case TypeKind::Real:
            genConstStore(std::stof(p_constant_value.value_str));
            break;
        case TypeKind::String:
            genConstStore(p_constant_value.value_str);
            break;
        case TypeKind::Void:
            break;
    }
}

void CodeGenerator::visit(FunctionNode &p_function) {
    symbol_mgr.pushScope(p_function.getSymTab());
    auto symTab = symbol_mgr.currentSymTab();

    genFunctionPrologue(p_function.getNameStr());
    // push first 8 param from register to local stack
    int total_param_num = 0;
    for (auto &p : p_function.parameters) {
        for (auto &var : p->var_list) {
            auto symbol = symTab->lookup(var->getNameStr());
            genParamLoad(total_param_num, symbol->getFpOffset(), symbol->getType());
            total_param_num++;
            if (total_param_num > 7) break;
        }
        if (total_param_num > 7) break;
    }
    p_function.compound_stmt->accept(*this);
    // gen return if not procedure
    if (p_function.return_type->kind != TypeKind::Void) {
        genReturn();
    }
    genFunctionEpilogue(p_function.getNameStr());

    symbol_mgr.popScope();
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
    genPrint(p_print.expr->getType());
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);
    genBinaryOperation(p_bin_op.op, p_bin_op.lexpr->getType(), p_bin_op.rexpr->getType());
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    p_un_op.visitChildNodes(*this);
    genUnaryOperation(p_un_op.op, p_un_op.expr->getType());
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    auto symTab = symbol_mgr.currentSymTab();
    auto symbol = symTab->lookup(p_func_invocation.name);
    assert(symbol->getKind() == SymbolEntryKind::Function && "Not call to function");
    auto retType = symbol->getType();

    // push arguments from back
    int arg_len = p_func_invocation.expressions.size();
    for (int i = arg_len - 1; i >= 0; i--) {
        p_func_invocation.expressions[i]->accept(*this);
    }
    // only pop first 8 arguments to register, the other will still stay in stack
    // so in callee context, use 0(fp) to get 9th args, 4(fp) to get 10th args, and so on.
    for (int i = 0; i < std::min(8, arg_len); i++) {
        genParamStore(i, p_func_invocation.expressions[i]->getType());
    }
    genFuncCall(p_func_invocation.name);
    // clean the remaining arguments in stack
    if (arg_len > 8) {
        genStackPop(arg_len - 8);
    }
    // store return value from register to stack
    if (retType->kind != TypeKind::Void) {
        genReturnValStore();
    }
}

void CodeGenerator::visit(FunctionCallNode &p_func_call) {
    p_func_call.visitChildNodes(*this);
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    auto symTab = symbol_mgr.currentSymTab();
    auto varName = p_variable_ref.name;
    auto symbol = symTab->lookup(varName);

    switch (p_variable_ref.getSide()) {
        case Side::LHS:
            if (symbol->getLevel() == 0) { // global var ref
                genGlobalVarAddrLoad(varName, symbol->getType(), p_variable_ref.expressions);
            } else { // local var ref
                genLocalVarAddrLoad(symbol->getFpOffset(),
                                    symbol->getKind(),
                                    symbol->getType(),
                                    p_variable_ref.expressions);
            }
            break;
        case Side::RHS:
            if (symbol->getLevel() == 0) { // global var ref
                genGlobalVarLoad(varName, symbol->getType(), p_variable_ref.expressions);
            } else { // local var ref
                genLocalVarLoad(symbol->getFpOffset(),
                                symbol->getKind(),
                                symbol->getType(),
                                p_variable_ref.expressions);
            }
            break;
    }
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    p_assignment.visitChildNodes(*this);

    auto symTab = symbol_mgr.currentSymTab();
    auto symbol = symTab->lookup(p_assignment.var_ref->name);
    genAssign(symbol->getType());
}

void CodeGenerator::visit(ReadNode &p_read) {
    p_read.visitChildNodes(*this);
    genRead();
}

void CodeGenerator::visit(IfNode &p_if) {
    int label_true = getLabel();
    int label_false = getLabel();
    int label_end = getLabel();

    p_if.condition->accept(*this);
    genIfFalseBranch(label_false);
    genLabel(label_true);
    p_if.body->accept(*this);
    genJump(label_end);
    genLabel(label_false);
    if (p_if.body_else != nullptr) {
        p_if.body_else->accept(*this);
    }
    genLabel(label_end);
}

void CodeGenerator::visit(WhileNode &p_while) {
    int loop_head = getLabel();
    int loop_body = getLabel();
    int loop_end = getLabel();

    genLabel(loop_head);
    p_while.condition->accept(*this);
    genIfFalseBranch(loop_end);
    genLabel(loop_body);
    p_while.body->accept(*this);
    genJump(loop_head);
    genLabel(loop_end);
}

void CodeGenerator::visit(ForNode &p_for) {
    int loop_head = getLabel();
    int loop_body = getLabel();
    int loop_end = getLabel();

    symbol_mgr.pushScope(p_for.getSymTab());
    auto symTab = symbol_mgr.currentSymTab();
    auto symbol = symTab->lookup(p_for.assignment->var_ref->name);

    p_for.declaration->accept(*this);
    p_for.assignment->accept(*this);
    genLabel(loop_head);
    // compare with condition constant
    genLocalVarLoad(symbol->getFpOffset(), symbol->getKind(), symbol->getType(),
                    std::vector<std::shared_ptr<ExpressionBase>>());
    p_for.condition->accept(*this);
    genBinaryOperation(BinaryOP::LESS, symbol->getType(), p_for.condition->getType());
    genIfFalseBranch(loop_end);
    // loop body
    genLabel(loop_body);
    p_for.compound_stmt->accept(*this);
    // increment loop var
    genLocalVarAddrLoad(symbol->getFpOffset(), symbol->getKind(), symbol->getType(),
                        std::vector<std::shared_ptr<ExpressionBase>>());
    genLocalVarLoad(symbol->getFpOffset(), symbol->getKind(), symbol->getType(),
                    std::vector<std::shared_ptr<ExpressionBase>>());
    genConstStore(1);
    genBinaryOperation(BinaryOP::PLUS, symbol->getType(), type_mgr.getType(TypeKind::Integer));
    genAssign(symbol->getType());
    // loop end
    genJump(loop_head);
    genLabel(loop_end);

    symbol_mgr.popScope();
}

void CodeGenerator::visit(ReturnNode &p_return) {
    p_return.visitChildNodes(*this);
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
                << "    sw t0, 116(sp)\n"
                << "    sw t1, 112(sp)\n"
                << "    sw t2, 108(sp)\n"
                << "    addi s0, sp, 128\n"
                << "    # in the function body\n";
}

void CodeGenerator::genFunctionEpilogue(std::string func_name) {
    output_file << "    # in the function epilogue\n"
                << "    lw ra, 124(sp)\n"
                << "    lw s0, 120(sp)\n"
                << "    lw t0, 116(sp)\n"
                << "    lw t1, 112(sp)\n"
                << "    lw t2, 108(sp)\n"
                << "    addi sp, sp, 128\n"
                << "    jr ra\n"
                << "    .size " << func_name << ", .-" << func_name << "\n";
}

void CodeGenerator::genGlobalVarDecl(std::string var_name, int bytes, int align) {
    output_file << "    .comm " << var_name << ", " << bytes << ", " << align << "\n";
}

void CodeGenerator::genGlobalVarAddrLoad(
    std::string var_name,
    const std::shared_ptr<TypeStruct> decl_type,
    const std::vector<std::shared_ptr<ExpressionBase>> exprs) {

    genPushGlobalVarAddr(var_name);
    genCaclArrayOffset(decl_type, exprs);
}

void CodeGenerator::genGlobalVarLoad(
    std::string var_name,
    const std::shared_ptr<TypeStruct> decl_type,
    const std::vector<std::shared_ptr<ExpressionBase>> exprs) {

    genGlobalVarAddrLoad(var_name, decl_type, exprs);
    genStackTopAddrToValue(decl_type);
}

void CodeGenerator::genLocalVarAddrLoad(
    int fp_offset,
    SymbolEntryKind decl_kind,
    const std::shared_ptr<TypeStruct> decl_type,
    const std::vector<std::shared_ptr<ExpressionBase>> exprs) {

    genPushLocalVarAddr(fp_offset);
    // load array address if var decl is a param and array
    if (decl_kind == SymbolEntryKind::Parameter && decl_type->isArray()) {
        genStackTopAddrToValue(decl_type);
    }
    genCaclArrayOffset(decl_type, exprs);
}

void CodeGenerator::genLocalVarLoad(
    int fp_offset,
    SymbolEntryKind decl_kind,
    const std::shared_ptr<TypeStruct> decl_type,
    const std::vector<std::shared_ptr<ExpressionBase>> exprs) {

    genLocalVarAddrLoad(fp_offset, decl_kind, decl_type, exprs);
    // load if not array after var ref
    if (decl_type->dim.size() == exprs.size()) {
        genStackTopAddrToValue(decl_type);
    }
}

void CodeGenerator::genStackTopAddrToValue(std::shared_ptr<TypeStruct> type) {
    output_file << "    # stack top addr load\n"
                << "    lw t0, 0(sp)\n";

    if (type->kind == TypeKind::Real) {
        output_file << "    flw ft1, 0(t0)\n"
                    << "    fsw ft1, 0(sp)\n";
    } else {
        output_file << "    lw t1, 0(t0)\n"
                    << "    sw t1, 0(sp)\n";
    }

}

void CodeGenerator::genPushGlobalVarAddr(std::string var_name) {
    output_file << "    # push global var addr\n"
                << "    la t0, " << var_name << "\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genPushLocalVarAddr(int fp_offset) {
    output_file << "    # push local var addr\n"
                << "    addi t0, s0, " << fp_offset << "\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genCaclArrayOffset(
    const std::shared_ptr<TypeStruct> decl_type,
    const std::vector<std::shared_ptr<ExpressionBase>> exprs) {

    if (!decl_type->isArray() || exprs.size() == 0) return;

    auto int_type = type_mgr.getType(TypeKind::Integer);
    for (auto i = 0UL; i < exprs.size(); i++) {
        auto &e = exprs[i];
        // is last dimension in declaration, no need to calculate offset
        if (i == decl_type->dim.size() - 1) {
            e->accept(*this); // push result of exprssion to stack
        } else {
            // calculate current dimension offset
            auto dim_offset = 1;
            for (auto j = i + 1; j < decl_type->dim.size(); j++) {
                dim_offset *= decl_type->dim[j];
            }
            genConstStore(dim_offset);
            // push result of exprssion to stack
            e->accept(*this);
            // multipy dimension offset and expression result
            genBinaryOperation(BinaryOP::MULTIPLY, int_type, int_type);
        }
    }
    // add all offset
    for (auto i = 0UL; i < exprs.size() - 1; i++) {
        genBinaryOperation(BinaryOP::PLUS, int_type, int_type);
    }
    // left shift the result 2 bits
    genStackTopLeftShift(2);
    // add offset and base
    genBinaryOperation(BinaryOP::PLUS, int_type, int_type);
}

void CodeGenerator::genStackTopLeftShift(int bits) {
    output_file << "    lw t0, 0(sp)\n"
                << "    slli t0, t0, 2\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genConstStore(int val) {
    output_file << "    # const int store\n"
                << "    li t0, " << val << "\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genConstStore(float val) {
    auto val_id = symbol_mgr.getRoDataIdentifier(val);

    output_file << "    # const float store\n"
                << "    lui t0, %hi(" << val_id << ")\n"
                << "    flw ft0, %lo(" << val_id << ")(t0)\n"
                << "    addi sp, sp, -4\n"
                << "    fsw ft0, 0(sp)\n";
}

void CodeGenerator::genConstStore(std::string val) {
    auto val_id = symbol_mgr.getRoDataIdentifier(val);

    output_file << "    # const string store\n"
                << "    lui t0, %hi(" << val_id << ")\n"
                << "    addi t1, t0, %lo(" << val_id << ")\n"
                << "    addi sp, sp, -4\n"
                << "    sw t1, 0(sp)\n";
}

void CodeGenerator::genAssign(std::shared_ptr<TypeStruct> type) {
    output_file << "    # var assign\n";
    if (type->kind == TypeKind::Real) {
        output_file << "    flw ft0, 0(sp)\n";
    } else {
        output_file << "    lw t0, 0(sp)\n";
    }
    output_file << "    addi sp, sp, 4\n"
                << "    lw t1, 0(sp)\n"
                << "    addi sp, sp, 4\n";
    if (type->kind == TypeKind::Real) {
        output_file << "    fsw ft0, 0(t1)\n";
    } else {
        output_file << "    sw t0, 0(t1)\n";
    }
}

void CodeGenerator::genGlobalVarConst(std::string var_name, std::string val_str) {
    output_file << ".section    .rodata\n"
                << "    .align 2\n"
                << "    .globl " << var_name << "\n"
                << "    .type " << var_name << ", @object\n"
                << var_name << ":\n"
                << "    .word " << val_str << "\n";
}

void CodeGenerator::genRoData(std::string sval, std::string sval_id) {
    output_file << ".section    .rodata\n"
                << "    .align 2\n"
                << sval_id << ":\n"
                << "    .string \"" << sval << "\"\n";
}

void CodeGenerator::genRoData(float rval, std::string rval_id) {
    output_file << ".section    .rodata\n"
                << "    .align 2\n"
                << rval_id << ":\n"
                << "    .float " << rval << "\n";
}


void CodeGenerator::genParamLoad(int param_num, int fp_offset, std::shared_ptr<TypeStruct> param_type) {
    output_file << "    # param load\n";
    if (param_type->kind == TypeKind::Real) {
        output_file << "    fsw fa" << param_num << ", " << fp_offset << "(s0)\n";
    } else {
        output_file << "    sw a" << param_num << ", " << fp_offset << "(s0)\n";
    }

}

void CodeGenerator::genParamStore(int param_num, std::shared_ptr<TypeStruct> param_type) {
    output_file << "    # param store\n";
    if (param_type->kind == TypeKind::Real) {
        output_file << "    flw fa" << param_num << ", 0(sp)\n";
    } else {
        output_file << "    lw a" << param_num << ", 0(sp)\n";
    }
    output_file << "    addi sp, sp, 4\n";
}

void CodeGenerator::genFuncCall(std::string func_name) {
    output_file << "    # function call\n"
                << "    jal ra, " << func_name << "\n";
}

void CodeGenerator::genReturn() {
    output_file << "    # return\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    mv a0, t0\n";
}

void CodeGenerator::genReturnValStore() {
    output_file << "    # return val store\n"
                << "    mv t0, a0\n"
                << "    addi sp, sp, -4\n"
                << "    sw t0, 0(sp)\n";
}

void CodeGenerator::genPrint(std::shared_ptr<TypeStruct> type) {
    output_file << "    # print\n";
    if (type->kind == TypeKind::Real) {
        output_file << "    flw fa0, 0(sp)\n";
    } else {
        output_file << "    lw a0, 0(sp)\n";
    }
    output_file << "    addi sp, sp, 4\n";
    switch (type->kind) {
        case TypeKind::Boolean:
        case TypeKind::Integer:
            output_file << "    jal ra, printInt\n";
            break;
        case TypeKind::Real:
            output_file << "    jal ra, printReal\n";
            break;
        case TypeKind::String:
            output_file << "    jal ra, printString\n";
            break;
        case TypeKind::Void:
            break;
    }
}

void CodeGenerator::genRead() {
    output_file << "    # read\n"
                << "    jal ra, readInt\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    sw a0, 0(t0)\n";
}

void CodeGenerator::genStackPop(int bytes) {
    output_file << "    # stack pop\n"
                << "    addi sp, sp, " << 4 * bytes << "\n";
}

void CodeGenerator::genBinaryOperation(
    BinaryOP op,
    std::shared_ptr<TypeStruct> ltype,
    std::shared_ptr<TypeStruct> rtype) {

    auto genNormal = [this, &op](){
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
    };

    auto genReal = [this, &op](){
        switch (op) {
            case BinaryOP::PLUS:
                output_file << "    fadd.s ft0, ft1, ft0\n"
                            << "    addi sp, sp, -4\n"
                            << "    fsw ft0, 0(sp)\n";
                break;
            case BinaryOP::MINUS:
                output_file << "    fsub.s ft0, ft1, ft0\n"
                            << "    addi sp, sp, -4\n"
                            << "    fsw ft0, 0(sp)\n";
                break;
            case BinaryOP::MULTIPLY:
                output_file << "    fmul.s ft0, ft1, ft0\n"
                            << "    addi sp, sp, -4\n"
                            << "    fsw ft0, 0(sp)\n";
                break;
            case BinaryOP::DIVIDE:
                output_file << "    fdiv.s ft0, ft1, ft0\n"
                            << "    addi sp, sp, -4\n"
                            << "    fsw ft0, 0(sp)\n";
                break;
            case BinaryOP::LESS:
                output_file << "    flt.s t0, ft1, ft0\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case BinaryOP::LESS_OR_EQUAL:
                output_file << "    fle.s t0, ft1, ft0\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case BinaryOP::GREATER:
                output_file << "    fle.s t0, ft1, ft0\n"
                            << "    xori t0, t0, 1\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case BinaryOP::GREATER_OR_EQUAL:
                output_file << "    flt.s t0, ft1, ft0\n"
                            << "    xori t0, t0, 1\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case BinaryOP::EQUAL:
                output_file << "    feq.s t0, ft1, ft0\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case BinaryOP::NOT_EQUAL:
                output_file << "    feq.s t0, ft1, ft0\n"
                            << "    xori t0, t0, 1\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case BinaryOP::MOD:
            case BinaryOP::AND:
            case BinaryOP::OR:
                std::cerr << "Unsupported binary opeartion for floating-point\n";
                break;
        }
    };

    // prepare register
    if (ltype->kind != rtype->kind) {
        if (coerce(ltype, rtype) == ltype) { // coerce right type to left type
            output_file << "    lw t0, 0(sp)\n"
                        << "    addi sp, sp, 4\n"
                        << "    fcvt.s.w ft0, t0\n"
                        << "    flw ft1, 0(sp)\n"
                        << "    addi sp, sp, 4\n";
        } else { // coerce left type to right type
            output_file << "    flw ft0, 0(sp)\n"
                        << "    addi sp, sp, 4\n"
                        << "    lw t1, 0(sp)\n"
                        << "    addi sp, sp, 4\n"
                        << "    fcvt.s.w ft1, t1\n";
        }
        genReal();
    } else if (ltype->kind == TypeKind::Real) {
        output_file << "    flw ft0, 0(sp)\n"
                    << "    addi sp, sp, 4\n"
                    << "    flw ft1, 0(sp)\n"
                    << "    addi sp, sp, 4\n";
        genReal();
    } else {
        output_file << "    lw t0, 0(sp)\n"
                    << "    addi sp, sp, 4\n"
                    << "    lw t1, 0(sp)\n"
                    << "    addi sp, sp, 4\n";
        genNormal();
    }
}

void CodeGenerator::genUnaryOperation(UnaryOP op, std::shared_ptr<TypeStruct> type) {
    auto genNormal = [this, &op](){
        output_file << "    # unary operation\n"
                    << "    lw t0, 0(sp)\n"
                    << "    addi sp, sp, 4\n";
        switch (op) {
            case UnaryOP::MINUS:
                output_file << "    neg t0, t0\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
            case UnaryOP::NOT:
                output_file << "    seqz t0, t0\n"
                            << "    andi t0, t0, 0xff\n"
                            << "    addi sp, sp, -4\n"
                            << "    sw t0, 0(sp)\n";
                break;
        }
    };

    auto genReal = [this, &op](){
        output_file << "    # unary operation\n"
                    << "    flw ft0, 0(sp)\n"
                    << "    addi sp, sp, 4\n";
        switch (op) {
            case UnaryOP::MINUS:
                output_file << "    fneg.s ft0, ft0\n"
                            << "    addi sp, sp, -4\n"
                            << "    fsw ft0, 0(sp)\n";
                break;
            case UnaryOP::NOT:
                std::cerr << "Unsupported unary opeartion for floating-point\n";
                break;
        }
    };

    if (type->kind == TypeKind::Real) {
        genReal();
    } else {
        genNormal();
    }
}

void CodeGenerator::genIfFalseBranch(int label) {
    output_file << "    # branch if false\n"
                << "    lw t0, 0(sp)\n"
                << "    addi sp, sp, 4\n"
                << "    beqz t0, L" << label << "\n";
}

void CodeGenerator::genLabel(int label) {
    output_file << "L" << label << ":\n";
}

void CodeGenerator::genJump(int label) {
    output_file << "    j L" << label << "\n";
}

int CodeGenerator::getLabel() {
    label += 1;
    return label;
}

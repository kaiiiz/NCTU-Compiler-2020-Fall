#include "visitor/SemanticAnalyzer.hpp"

#include <iostream>
#include <vector>
#include <fstream>

#include "AST/decl.hpp"
#include "AST/expression/BinaryOperator.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "AST/expression/ConstantValue/ConstBoolValue.hpp"
#include "AST/expression/ConstantValue/ConstIntValue.hpp"
#include "AST/expression/ConstantValue/ConstRealValue.hpp"
#include "AST/expression/ConstantValue/ConstStrValue.hpp"
#include "AST/expression/FunctionInvocation.hpp"
#include "AST/expression/UnaryOperator.hpp"
#include "AST/expression/VariableReference.hpp"
#include "AST/function.hpp"
#include "AST/program.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/statement/FunctionCall.hpp"
#include "AST/statement/assignment.hpp"
#include "AST/statement/for.hpp"
#include "AST/statement/if.hpp"
#include "AST/statement/print.hpp"
#include "AST/statement/read.hpp"
#include "AST/statement/return.hpp"
#include "AST/statement/while.hpp"
#include "AST/variable.hpp"
#include "sema/SymbolEntry.hpp"
#include "sema/SymbolEntry/ConstSymbolEntry.hpp"
#include "sema/SymbolEntry/FunctionSymbolEntry.hpp"
#include "sema/SymbolEntry/LoopVarSymbolEntry.hpp"
#include "sema/SymbolEntry/ParamSymbolEntry.hpp"
#include "sema/SymbolEntry/ProgramSymbolEntry.hpp"
#include "sema/SymbolEntry/VarSymbolEntry.hpp"
#include "sema/SymbolManager.hpp"
#include "sema/SymbolTable.hpp"
#include "type/base.hpp"

SemanticAnalyzer::SemanticAnalyzer(SymbolManager &symbol_mgr, std::vector<long> &line_head, std::string source_filename)
    : symbol_mgr(symbol_mgr), line_head(line_head), source_filename(source_filename) {
}

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    // 1. Push a new symbol table if this node forms a scope.
    auto globalSymTab = symbol_mgr.currentSymTab();  // global symbol table
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    auto symbol = std::make_shared<ProgramSymbolEntry>(p_program.getProgramName(),
                                                       globalSymTab->level,
                                                       p_program.getReturnType(),
                                                       p_program.getLocation());
    insertWithCheck(globalSymTab, symbol);
    // 3. Travere child nodes of this node.
    p_program.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    // 1. Push a new symbol table if this node forms a scope.
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_decl.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    std::shared_ptr<SymbolEntry> symbol;
    switch (p_variable.getVarKind()) {
    case VariableKind::parameter:
        symbol = std::make_shared<ParamSymbolEntry>(p_variable.getNameStr(),
                                                    symTab->level,
                                                    p_variable.getType(),
                                                    p_variable.getLocation());
        break;
    case VariableKind::constant:
        switch (p_variable.getType()->getTypeKind()) {
        case TypeKind::boolean:
            symbol = std::make_shared<ConstBoolSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstBoolValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::integer:
            symbol = std::make_shared<ConstIntSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstIntValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::real:
            symbol = std::make_shared<ConstRealSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstRealValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::string:
            symbol = std::make_shared<ConstStrSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstStrValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::void_:
            break;
        }
        break;
    case VariableKind::variable:
        symbol = std::make_shared<VarSymbolEntry>(p_variable.getNameStr(),
                                                  symTab->level,
                                                  p_variable.getType(),
                                                  p_variable.getLocation());
        break;
    case VariableKind::loop_var:
        symbol = std::make_shared<LoopVarSymbolEntry>(p_variable.getNameStr(),
                                                      symTab->level,
                                                      p_variable.getType(),
                                                      p_variable.getLocation());
        break;
    }
    insertWithCheck(symTab, symbol);
    // 3. Travere child nodes of this node.
    p_variable.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_constant_value.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    auto symbol = std::make_shared<FunctionSymbolEntry>(p_function.getNameStr(),
                                                        symTab->level,
                                                        p_function.getRetType(),
                                                        p_function.getParamTypeList(),
                                                        p_function.getLocation());
    insertWithCheck(symTab, symbol);
    auto newSymTab = std::make_shared<SymbolTable>(symTab, symTab->level + 1);
    symTab->addChild(newSymTab);
    symbol_mgr.pushScope(newSymTab);
    // 3. Travere child nodes of this node.
    p_function.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // 5. Pop the symbol table pushed at the 1st step.
    symbol_mgr.popScope();
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    // 1. Push a new symbol table if this node forms a scope.
    if (p_compound_statement.getKind() == CompoundKind::normal) {
        auto curSymTab = symbol_mgr.currentSymTab();
        auto symTab = std::make_shared<SymbolTable>(curSymTab, curSymTab->level + 1);
        curSymTab->addChild(symTab);
        symbol_mgr.pushScope(symTab);
    }
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_compound_statement.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // 5. Pop the symbol table pushed at the 1st step.
    if (p_compound_statement.getKind() == CompoundKind::normal) {
        symbol_mgr.popScope();
    }
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_print.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_bin_op.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_un_op.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_func_invocation.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_variable_ref.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_assignment.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_read.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_if.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_while.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    // 1. Push a new symbol table if this node forms a scope.
    auto curSymTab = symbol_mgr.currentSymTab();
    auto symTab = std::make_shared<SymbolTable>(curSymTab, curSymTab->level + 1);
    curSymTab->addChild(symTab);
    symbol_mgr.pushScope(symTab);
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_for.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // 5. Pop the symbol table pushed at the 1st step.
    symbol_mgr.popScope();
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_return.visitChildNodes(*this);
}

void SemanticAnalyzer::insertWithCheck(std::shared_ptr<SymbolTable> sym_tab,
                                       std::shared_ptr<SymbolEntry> symbol) {
    std::string symbol_name = symbol->getNameStr();
    // normal symbol redeclaration check
    if (sym_tab->lookup(symbol_name) != nullptr) {
        fprintf(stderr, "<Error> Found in line %u, column %u: symbol '%s' is redeclared\n"
                        "   %s\n"
                        "   %s\n",
                        symbol->location.line, symbol->location.col, symbol->getNameStr().c_str(),
                        getSourceLine(symbol->location.line).c_str(),
                        getErrIndicator(symbol->location.col).c_str());
        return;
    }
    sym_tab->insert(symbol);
}

std::string SemanticAnalyzer::getSourceLine(long lineno) {
    std::ifstream source_file(source_filename);
    std::string line;
    if (!source_file.good()) {
        std::cerr << "[SemanticAnalyzer] File stream is not good" << std::endl;
        exit(EXIT_FAILURE);
    }
    source_file.seekg(line_head[lineno - 1]);
    std::getline(source_file, line);
    return line;
}

std::string SemanticAnalyzer::getErrIndicator(long col) {
    std::string err_indicator;
    for (int i = 0; i < col - 1; i++) err_indicator += " ";
    err_indicator += "^";
    return err_indicator;
}

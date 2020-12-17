#include "visitor/SemanticAnalyzer.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

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

#include "type/struct.hpp"

static inline bool is_void(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::void_; }
static inline bool is_real(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::real; }
static inline bool is_int(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::integer; }
static inline bool is_bool(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::boolean; }
static inline bool is_str(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::string; }

SemanticAnalyzer::SemanticAnalyzer(SymbolManager &symbol_mgr, std::vector<long> &line_head, std::string source_filename)
    : symbol_mgr(symbol_mgr), line_head(line_head), source_filename(source_filename) {}

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    // 1. Push a new symbol table if this node forms a scope.
    auto globalSymTab = symbol_mgr.currentSymTab();
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
        switch (p_variable.getType()->kind) {
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
    auto var_type = p_variable.getType();
    if (var_type->isArray()) {
        for (auto &d : var_type->dim) {
            if (d <= 0) {
                fprintf(stderr, "<Error> Found in line %u, column %u: '%s' declared as an array with an index that is not greater than 0\n"
                                "    %s\n"
                                "    %s\n",
                                p_variable.getLocation().line, p_variable.getLocation().col, p_variable.getNameStr().c_str(),
                                getSourceLine(p_variable.getLocation().line).c_str(),
                                getErrIndicator(p_variable.getLocation().col).c_str());
                recordError(p_variable.getLocation().line, p_variable.getLocation().col);
                return;
            }
        }
    }
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
    auto newSymTab = std::make_shared<SymbolTable>(symTab, symTab->level + 1);
    symTab->addChild(newSymTab);
    insertWithCheck(symTab, symbol);
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
    // 1. Push a new symbol table if this node forms a scope.
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_bin_op.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    auto lexpr = p_bin_op.getLExpr();
    auto rexpr = p_bin_op.getRExpr();
    auto lexpr_type = lexpr->getType();
    auto rexpr_type = rexpr->getType();
    // Skip the rest of semantic checks if there are any errors in the nodes of operands
    if (lexpr_type == nullptr || rexpr_type == nullptr) {
        return;
    }
    auto log_error = [this, &p_bin_op, &lexpr_type, &rexpr_type]() {
        fprintf(stderr, "<Error> Found in line %u, column %u: "
                        "invalid operands to binary operator '%s' ('%s' and '%s')\n"
                        "    %s\n"
                        "    %s\n",
                        p_bin_op.getLocation().line, p_bin_op.getLocation().col,
                        p_bin_op.getOPString().c_str(),
                        lexpr_type->getTypeStr().c_str(), rexpr_type->getTypeStr().c_str(),
                        getSourceLine(p_bin_op.getLocation().line).c_str(),
                        getErrIndicator(p_bin_op.getLocation().col).c_str());
        recordError(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
    };
    // two expression must be non void, non array, same type after coercion
    if (is_void(lexpr_type) || is_void(rexpr_type) ||
        lexpr_type->isArray() || rexpr_type->isArray() ||
        !typeEq(lexpr_type, rexpr_type)) {
        log_error();
        return;
    }

    switch (p_bin_op.op) {
    case BinaryOP::PLUS:
        if (!((is_int(lexpr_type) || is_real(lexpr_type) || is_str(lexpr_type)) &&
              (is_int(rexpr_type) || is_real(rexpr_type) || is_str(rexpr_type)))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(coerce(lexpr_type, rexpr_type));
        break;
    case BinaryOP::MINUS:
    case BinaryOP::MULTIPLY:
    case BinaryOP::DIVIDE:
        if (!((is_int(lexpr_type) || is_real(lexpr_type)) &&
              (is_int(rexpr_type) || is_real(rexpr_type)))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(coerce(lexpr_type, rexpr_type));
        break;
    case BinaryOP::MOD:
        if (!(is_int(lexpr_type) && is_int(rexpr_type))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(coerce(lexpr_type, rexpr_type));
        break;
    case BinaryOP::AND:
    case BinaryOP::OR:
        if (!(is_bool(lexpr_type) && is_bool(rexpr_type))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(std::make_shared<TypeStruct>(TypeKind::boolean));
        break;
    case BinaryOP::LESS:
    case BinaryOP::LESS_OR_EQUAL:
    case BinaryOP::EQUAL:
    case BinaryOP::GREATER_OR_EQUAL:
    case BinaryOP::GREATER:
    case BinaryOP::NOT_EQUAL:
        if (!((is_int(lexpr_type) || is_real(lexpr_type)) &&
              (is_int(rexpr_type) || is_real(rexpr_type)))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(std::make_shared<TypeStruct>(TypeKind::boolean));
        break;
    }

    // 5. Pop the symbol table pushed at the 1st step.
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
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_variable_ref.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    auto symbol = symTab->lookup(p_variable_ref.getNameStr());
    // The identifier has to be in symbol tables.
    if (symbol == nullptr) {
        fprintf(stderr, "<Error> Found in line %u, column %u: use of undeclared symbol '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_variable_ref.getLocation().line, p_variable_ref.getLocation().col,
                        p_variable_ref.getNameStr().c_str(),
                        getSourceLine(p_variable_ref.getLocation().line).c_str(),
                        getErrIndicator(p_variable_ref.getLocation().col).c_str());
        recordError(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        return;
    }
    // The kind of symbol has to be a parameter, variable, loop_var, or constant.
    else if (symbol->getKind() != SymbolEntryKind::parameter &&
             symbol->getKind() != SymbolEntryKind::variable &&
             symbol->getKind() != SymbolEntryKind::loop_var &&
             symbol->getKind() != SymbolEntryKind::constant) {
        fprintf(stderr, "<Error> Found in line %u, column %u: use of non-variable symbol '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_variable_ref.getLocation().line, p_variable_ref.getLocation().col,
                        p_variable_ref.getNameStr().c_str(),
                        getSourceLine(p_variable_ref.getLocation().line).c_str(),
                        getErrIndicator(p_variable_ref.getLocation().col).c_str());
        recordError(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        return;
    }
    // Skip the rest of semantic checks if there are any errors in the node of the declaration of the refered symbol
    if (hasErrorAt(symbol->getLocation().line, symbol->getLocation().col)) {
        return;
    }
    // Each index of an array reference must be of the integer type.
    for (auto &e : p_variable_ref.getExprs()) {
        if (e->getType()->kind != TypeKind::integer) {
            auto index_col = p_variable_ref.getLocation().col + p_variable_ref.getNameStr().length() + 1;
            fprintf(stderr, "<Error> Found in line %u, column %lu: index of array reference must be an integer\n"
                            "    %s\n"
                            "    %s\n",
                            p_variable_ref.getLocation().line, index_col,
                            getSourceLine(p_variable_ref.getLocation().line).c_str(),
                            getErrIndicator(index_col).c_str());
            recordError(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
            return;
        }
    }
    // An over array subscript is forbidden, that is, the number of indices of an array reference cannot be greater than the one of dimensions in the declaration.
    if (symbol->getType()->dim.size() < p_variable_ref.getExprs().size()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: there is an over array subscript on '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_variable_ref.getLocation().line, p_variable_ref.getLocation().col,
                        p_variable_ref.getNameStr().c_str(),
                        getSourceLine(p_variable_ref.getLocation().line).c_str(),
                        getErrIndicator(p_variable_ref.getLocation().col).c_str());
        recordError(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        return;
    }
    // back propagate type information

    // 5. Pop the symbol table pushed at the 1st step.
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

bool SemanticAnalyzer::insertWithCheck(std::shared_ptr<SymbolTable> sym_tab,
                                       std::shared_ptr<SymbolEntry> symbol) {
    auto symbol_name = symbol->getNameStr();
    auto exist_symbol = sym_tab->lookup(symbol_name); 
    // normal symbol redeclaration check
    if (exist_symbol != nullptr && exist_symbol->getLevel() == symbol->getLevel()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: symbol '%s' is redeclared\n"
                        "    %s\n"
                        "    %s\n",
                        symbol->getLocation().line, symbol->getLocation().col, symbol->getNameStr().c_str(),
                        getSourceLine(symbol->getLocation().line).c_str(),
                        getErrIndicator(symbol->getLocation().col).c_str());
        recordError(symbol->getLocation().line, symbol->getLocation().col);
        return false;
    }
    // loop var redeclaration check
    else if (exist_symbol != nullptr && exist_symbol->getKind() == SymbolEntryKind::loop_var) {
        fprintf(stderr, "<Error> Found in line %u, column %u: symbol '%s' is redeclared\n"
                        "    %s\n"
                        "    %s\n",
                        symbol->getLocation().line, symbol->getLocation().col, symbol->getNameStr().c_str(),
                        getSourceLine(symbol->getLocation().line).c_str(),
                        getErrIndicator(symbol->getLocation().col).c_str());
        recordError(symbol->getLocation().line, symbol->getLocation().col);
        return false;
    }
    sym_tab->insert(symbol);
    return true;
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

bool SemanticAnalyzer::hasError() {
    return !error_list.empty();
}

void SemanticAnalyzer::recordError(long lineno, long col) {
    error_list[lineno].push_back(col);
}

bool SemanticAnalyzer::hasErrorAt(long lineno, long col) {
    if (error_list.find(lineno) == error_list.end()) { return false; }
    auto &err_in_line = error_list[lineno];
    return std::find(err_in_line.begin(), err_in_line.end(), col) != err_in_line.end();
}

bool SemanticAnalyzer::typeEq(std::shared_ptr<TypeStruct> t1, std::shared_ptr<TypeStruct> t2) {
    if (t1->kind != t2->kind && coerce(t1, t2) == nullptr) return false;
    if (t1->isArray() && t2->isArray()) {
        if (t1->dim.size() != t2->dim.size()) return false;
        for (uint64_t i = 0; i < t1->dim.size(); i++) {
            if (t1->dim[i] != t2->dim[i]) return false;
        }
    }
    else if ((t1->isArray() && !t2->isArray()) || (t2->isArray() && !t1->isArray())) {
        return false;
    }
    return true;
}

std::shared_ptr<TypeStruct> SemanticAnalyzer::coerce(std::shared_ptr<TypeStruct> t1,
                                                     std::shared_ptr<TypeStruct> t2) {
    // Type coercion is not permitted for array
    if (t1->isArray() || t2->isArray()) return nullptr;

    if (t1->kind == TypeKind::real && t2->kind == TypeKind::integer) {
        return t1;        
    }
    else if (t2->kind == TypeKind::real && t1->kind == TypeKind::integer) {
        return t2;
    }
    return nullptr;
}

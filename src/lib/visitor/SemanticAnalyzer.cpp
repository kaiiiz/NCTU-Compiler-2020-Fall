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
#include "type/manager.hpp"

extern TypeManager type_mgr;

static inline bool is_void(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Void; }
static inline bool is_real(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Real; }
static inline bool is_int(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Integer; }
static inline bool is_bool(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Boolean; }
static inline bool is_str(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::String; }

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
    case VariableKind::Parameter:
        symbol = std::make_shared<ParamSymbolEntry>(p_variable.getNameStr(),
                                                    symTab->level,
                                                    p_variable.getType(),
                                                    p_variable.getLocation());
        break;
    case VariableKind::Constant:
        switch (p_variable.getType()->kind) {
        case TypeKind::Boolean:
            symbol = std::make_shared<ConstBoolSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstBoolValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::Integer:
            symbol = std::make_shared<ConstIntSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstIntValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::Real:
            symbol = std::make_shared<ConstRealSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstRealValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::String:
            symbol = std::make_shared<ConstStrSymbolEntry>(
                p_variable.getNameStr(), symTab->level, p_variable.getType(),
                p_variable.getLiteralConst<ConstStrValueNode>()->getValue(),
                p_variable.getLocation());
            break;
        case TypeKind::Void:
            break;
        }
        break;
    case VariableKind::Variable:
        symbol = std::make_shared<VarSymbolEntry>(p_variable.getNameStr(),
                                                  symTab->level,
                                                  p_variable.getType(),
                                                  p_variable.getLocation());
        break;
    case VariableKind::LoopVar:
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
    std::shared_ptr<SymbolTable> newSymTab;
    if (p_function.getRetType()->kind == TypeKind::Void) {
        newSymTab = std::make_shared<SymbolTable>(symTab, symTab->level + 1, ContextKind::Procedure, type_mgr.getType(TypeKind::Void));
    }
    else {
        newSymTab = std::make_shared<SymbolTable>(symTab, symTab->level + 1, ContextKind::Function, p_function.getRetType());
    }
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
        auto symTab = std::make_shared<SymbolTable>(curSymTab, curSymTab->level + 1, curSymTab->ctx_kind, curSymTab->ctx_type);
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
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_print.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // Skip the rest of semantic checks if there are any errors in the node of the expression (target)
    if (hasErrorAt(p_print.expr->getLocation().line, p_print.expr->getLocation().col)) {
        recordError(p_print.getLocation().line, p_print.getLocation().col);
        return;
    }
    // The type of the expression (target) must be scalar type
    if (p_print.expr->getType()->isArray() || p_print.expr->getType()->kind == TypeKind::Void) {
        fprintf(stderr, "<Error> Found in line %u, column %u: expression of print statement must be scalar type\n"
                        "    %s\n"
                        "    %s\n",
                        p_print.expr->getLocation().line, p_print.expr->getLocation().col,
                        getSourceLine(p_print.expr->getLocation().line).c_str(),
                        getErrIndicator(p_print.expr->getLocation().col).c_str());
        recordError(p_print.expr->getLocation().line, p_print.expr->getLocation().col);
        recordError(p_print.getLocation().line, p_print.getLocation().col);
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
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
    if (hasErrorAt(lexpr->getLocation().line, lexpr->getLocation().col) ||
        hasErrorAt(rexpr->getLocation().line, rexpr->getLocation().col)) {
        recordError(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
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
        if (lexpr_type->kind == rexpr_type->kind) {
            p_bin_op.fillAttribute(lexpr_type);
        } else if (coerce(lexpr_type, rexpr_type) != nullptr) {
            p_bin_op.fillAttribute(coerce(lexpr_type, rexpr_type));
        } else {
            log_error();
            return;
        }
        break;
    case BinaryOP::MINUS:
    case BinaryOP::MULTIPLY:
    case BinaryOP::DIVIDE:
        if (!((is_int(lexpr_type) || is_real(lexpr_type)) &&
              (is_int(rexpr_type) || is_real(rexpr_type)))) {
            log_error();
            return;
        }
        if (lexpr_type->kind == rexpr_type->kind) {
            p_bin_op.fillAttribute(lexpr_type);
        } else if (coerce(lexpr_type, rexpr_type) != nullptr) {
            p_bin_op.fillAttribute(coerce(lexpr_type, rexpr_type));
        } else {
            log_error();
            return;
        }
        break;
    case BinaryOP::MOD:
        if (!(is_int(lexpr_type) && is_int(rexpr_type))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(type_mgr.getType(TypeKind::Integer));
        break;
    case BinaryOP::AND:
    case BinaryOP::OR:
        if (!(is_bool(lexpr_type) && is_bool(rexpr_type))) {
            log_error();
            return;
        }
        p_bin_op.fillAttribute(type_mgr.getType(TypeKind::Boolean));
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
        p_bin_op.fillAttribute(type_mgr.getType(TypeKind::Boolean));
        break;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_un_op.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    auto expr_type = p_un_op.expr->getType();
    // Skip the rest of semantic checks if there are any errors in the nodes of operands
    if (hasErrorAt(p_un_op.expr->getLocation().line, p_un_op.expr->getLocation().col)) {
        recordError(p_un_op.getLocation().line, p_un_op.getLocation().col);
        return;
    }
    auto log_error = [this, &p_un_op, &expr_type]() {
        fprintf(stderr, "<Error> Found in line %u, column %u: "
                        "invalid operand to unary operator '%s' ('%s')\n"
                        "    %s\n"
                        "    %s\n",
                        p_un_op.getLocation().line, p_un_op.getLocation().col,
                        p_un_op.getOPString().c_str(),
                        expr_type->getTypeStr().c_str(),
                        getSourceLine(p_un_op.getLocation().line).c_str(),
                        getErrIndicator(p_un_op.getLocation().col).c_str());
        recordError(p_un_op.getLocation().line, p_un_op.getLocation().col);
    };
    // expression must be non void, non array
    if (is_void(expr_type) || expr_type->isArray()) {
        log_error();
        return;
    }

    switch (p_un_op.op) {
    case UnaryOP::MINUS:
        if (!(is_int(expr_type) || is_real(expr_type))) {
            log_error();
            return;
        }
        p_un_op.fillAttribute(expr_type);
        break;
    case UnaryOP::NOT:
        if (!(is_bool(expr_type))) {
            log_error();
            return;
        }
        p_un_op.fillAttribute(type_mgr.getType(TypeKind::Boolean));
        break;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_func_invocation.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    auto symbol = symTab->lookup(p_func_invocation.getFuncName());
    // The identifier has to be in symbol tables.
    if (symbol == nullptr) {
        fprintf(stderr, "<Error> Found in line %u, column %u: use of undeclared symbol '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_func_invocation.getLocation().line, p_func_invocation.getLocation().col,
                        p_func_invocation.getFuncName().c_str(),
                        getSourceLine(p_func_invocation.getLocation().line).c_str(),
                        getErrIndicator(p_func_invocation.getLocation().col).c_str());
        recordError(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        return;
    }
    // The kind of symbol has to be function.
    else if (symbol->getKind() != SymbolEntryKind::Function) {
        fprintf(stderr, "<Error> Found in line %u, column %u: call of non-function symbol '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_func_invocation.getLocation().line, p_func_invocation.getLocation().col,
                        p_func_invocation.getFuncName().c_str(),
                        getSourceLine(p_func_invocation.getLocation().line).c_str(),
                        getErrIndicator(p_func_invocation.getLocation().col).c_str());
        recordError(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        return;
    }
    // Pointer cast to function symbol after checking symbol kind is function
    auto func_symbol = std::dynamic_pointer_cast<FunctionSymbolEntry>(symbol);
    // The number of arguments must be the same as one of the parameters.
    if (func_symbol->getParamType().size() != p_func_invocation.getExprs().size()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: too few/much arguments provided for function '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_func_invocation.getLocation().line, p_func_invocation.getLocation().col,
                        p_func_invocation.getFuncName().c_str(),
                        getSourceLine(p_func_invocation.getLocation().line).c_str(),
                        getErrIndicator(p_func_invocation.getLocation().col).c_str());
        recordError(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
        return;
    }
    // Traverse arguments
    auto param_prototype = func_symbol->getParamType();
    auto arg_exprs = p_func_invocation.getExprs();
    for (uint64_t i = 0; i < param_prototype.size(); i++) {
        auto param_type = param_prototype[i];
        auto arg_expr = arg_exprs[i];
        if (!typeEq(param_type, arg_expr->getType())) {
            fprintf(stderr, "<Error> Found in line %u, column %u: incompatible type passing '%s' to parameter of type '%s'\n"
                            "    %s\n"
                            "    %s\n",
                            arg_expr->getLocation().line, arg_expr->getLocation().col,
                            arg_expr->getType()->getTypeStr().c_str(), param_type->getTypeStr().c_str(),
                            getSourceLine(arg_expr->getLocation().line).c_str(),
                            getErrIndicator(arg_expr->getLocation().col).c_str());
            recordError(arg_expr->getLocation().line, arg_expr->getLocation().col);
            recordError(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col);
            return;
        }
    }
    p_func_invocation.fillAttribute(func_symbol->getType());
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(FunctionCallNode &p_func_call) {
    p_func_call.visitChildNodes(*this);
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
    else if (symbol->getKind() != SymbolEntryKind::Parameter &&
             symbol->getKind() != SymbolEntryKind::Variable &&
             symbol->getKind() != SymbolEntryKind::LoopVar &&
             symbol->getKind() != SymbolEntryKind::Constant) {
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
        recordError(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
        return;
    }
    // Each index of an array reference must be of the integer type.
    for (auto &e : p_variable_ref.getExprs()) {
        if (hasErrorAt(e->getLocation().line, e->getLocation().col)) {
            recordError(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
            return;
        }
        if (e->getType()->kind != TypeKind::Integer) {
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
    p_variable_ref.fillAttribute(type_mgr.getType(symbol->getType(), p_variable_ref.getExprs().size()));
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_assignment.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // Skip the rest of semantic checks if there are any errors in the node of the variable reference (lvalue)
    if (hasErrorAt(p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col)) {
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }
    // The type of the result of the variable reference cannot be an array type
    if (p_assignment.var_ref->getType()->isArray()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: array assignment is not allowed\n"
                        "    %s\n"
                        "    %s\n",
                        p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col,
                        getSourceLine(p_assignment.var_ref->getLocation().line).c_str(),
                        getErrIndicator(p_assignment.var_ref->getLocation().col).c_str());
        recordError(p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col);
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }
    // The variable reference cannot be a reference to a constant variable
    auto symbol = symTab->lookup(p_assignment.var_ref->getNameStr());
    if (symbol->getKind() == SymbolEntryKind::Constant) {
        fprintf(stderr, "<Error> Found in line %u, column %u: cannot assign to variable '%s' which is a constant\n"
                        "    %s\n"
                        "    %s\n",
                        p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col,
                        p_assignment.var_ref->getNameStr().c_str(),
                        getSourceLine(p_assignment.var_ref->getLocation().line).c_str(),
                        getErrIndicator(p_assignment.var_ref->getLocation().col).c_str());
        recordError(p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col);
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }
    // The variable reference cannot be a reference to a loop variable when the context is within a loop body
    if (symbol->getKind() == SymbolEntryKind::LoopVar && symTab->level > symbol->getLevel()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: the value of loop variable cannot be modified inside the loop body\n"
                        "    %s\n"
                        "    %s\n",
                        p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col,
                        getSourceLine(p_assignment.var_ref->getLocation().line).c_str(),
                        getErrIndicator(p_assignment.var_ref->getLocation().col).c_str());
        recordError(p_assignment.var_ref->getLocation().line, p_assignment.var_ref->getLocation().col);
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }

    // Skip the rest of semantic checks if there are any errors in the node of the expression
    if (hasErrorAt(p_assignment.expression->getLocation().line, p_assignment.expression->getLocation().col)) {
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }
    // The type of the result of the expression cannot be an array type
    if (p_assignment.expression->getType()->isArray()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: array assignment is not allowed\n"
                        "    %s\n"
                        "    %s\n",
                        p_assignment.expression->getLocation().line, p_assignment.expression->getLocation().col,
                        getSourceLine(p_assignment.expression->getLocation().line).c_str(),
                        getErrIndicator(p_assignment.expression->getLocation().col).c_str());
        recordError(p_assignment.expression->getLocation().line, p_assignment.expression->getLocation().col);
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }
    // The type of the variable reference (lvalue) must be the same as the one of the expression after appropriate type coercion
    else if (!typeEq(p_assignment.var_ref->getType(), p_assignment.expression->getType())) {
        fprintf(stderr, "<Error> Found in line %u, column %u: assigning to '%s' from incompatible type '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_assignment.getLocation().line, p_assignment.getLocation().col,
                        p_assignment.var_ref->getType()->getTypeStr().c_str(),
                        p_assignment.expression->getType()->getTypeStr().c_str(),
                        getSourceLine(p_assignment.getLocation().line).c_str(),
                        getErrIndicator(p_assignment.getLocation().col).c_str());
        recordError(p_assignment.getLocation().line, p_assignment.getLocation().col);
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_read.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // Skip the rest of semantic checks if there are any errors in the node of the variable reference
    if (hasErrorAt(p_read.var_ref->getLocation().line, p_read.var_ref->getLocation().col)) {
        recordError(p_read.getLocation().line, p_read.getLocation().col);
        return;
    }
    // The type of the variable reference must be scalar type
    if (p_read.var_ref->getType()->isArray()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: variable reference of read statement must be scalar type\n"
                        "    %s\n"
                        "    %s\n",
                        p_read.var_ref->getLocation().line, p_read.var_ref->getLocation().col,
                        getSourceLine(p_read.var_ref->getLocation().line).c_str(),
                        getErrIndicator(p_read.var_ref->getLocation().col).c_str());
        recordError(p_read.var_ref->getLocation().line, p_read.var_ref->getLocation().col);
        recordError(p_read.getLocation().line, p_read.getLocation().col);
        return;
    }
    // The kind of symbol of the variable reference cannot be constant or loop_var
    auto symbol = symTab->lookup(p_read.var_ref->getNameStr());
    if (symbol->getKind() == SymbolEntryKind::Constant || symbol->getKind() == SymbolEntryKind::LoopVar) {
        fprintf(stderr, "<Error> Found in line %u, column %u: variable reference of read statement cannot be a constant or loop variable\n"
                        "    %s\n"
                        "    %s\n",
                        p_read.var_ref->getLocation().line, p_read.var_ref->getLocation().col,
                        getSourceLine(p_read.var_ref->getLocation().line).c_str(),
                        getErrIndicator(p_read.var_ref->getLocation().col).c_str());
        recordError(p_read.var_ref->getLocation().line, p_read.var_ref->getLocation().col);
        recordError(p_read.getLocation().line, p_read.getLocation().col);
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_if.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // Skip the rest of semantic checks if there are any errors in the node of the expression (condition)
    if (hasErrorAt(p_if.condition->getLocation().line, p_if.condition->getLocation().col)) {
        recordError(p_if.getLocation().line, p_if.getLocation().col);
        return;
    }
    // The type of the result of the expression (condition) must be boolean type
    if (p_if.condition->getType()->kind != TypeKind::Boolean) {
        fprintf(stderr, "<Error> Found in line %u, column %u: the expression of condition must be boolean type\n"
                        "    %s\n"
                        "    %s\n",
                        p_if.condition->getLocation().line, p_if.condition->getLocation().col,
                        getSourceLine(p_if.condition->getLocation().line).c_str(),
                        getErrIndicator(p_if.condition->getLocation().col).c_str());
        recordError(p_if.condition->getLocation().line, p_if.condition->getLocation().col);
        recordError(p_if.getLocation().line, p_if.getLocation().col);
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_while.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // Skip the rest of semantic checks if there are any errors in the node of the expression (condition)
    if (hasErrorAt(p_while.condition->getLocation().line, p_while.condition->getLocation().col)) {
        recordError(p_while.getLocation().line, p_while.getLocation().col);
        return;
    }
    // The type of the result of the expression (condition) must be boolean type
    if (p_while.condition->getType()->kind != TypeKind::Boolean) {
        fprintf(stderr, "<Error> Found in line %u, column %u: the expression of condition must be boolean type\n"
                        "    %s\n"
                        "    %s\n",
                        p_while.condition->getLocation().line, p_while.condition->getLocation().col,
                        getSourceLine(p_while.condition->getLocation().line).c_str(),
                        getErrIndicator(p_while.condition->getLocation().col).c_str());
        recordError(p_while.condition->getLocation().line, p_while.condition->getLocation().col);
        recordError(p_while.getLocation().line, p_while.getLocation().col);
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    // 1. Push a new symbol table if this node forms a scope.
    auto curSymTab = symbol_mgr.currentSymTab();
    auto symTab = std::make_shared<SymbolTable>(curSymTab, curSymTab->level + 1, curSymTab->ctx_kind, curSymTab->ctx_type);
    curSymTab->addChild(symTab);
    symbol_mgr.pushScope(symTab);
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_for.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // The initial value of the loop variable and the constant value of the condition must be in the incremental order
    auto initial_node = std::dynamic_pointer_cast<ConstIntValueNode>(p_for.assignment->expression);
    auto end_node = std::dynamic_pointer_cast<ConstIntValueNode>(p_for.condition);
    if (initial_node->getValue() > end_node->getValue()) {
        fprintf(stderr, "<Error> Found in line %u, column %u: the lower bound and upper bound of iteration count must be in the incremental order\n"
                        "    %s\n"
                        "    %s\n",
                        p_for.getLocation().line, p_for.getLocation().col,
                        getSourceLine(p_for.getLocation().line).c_str(),
                        getErrIndicator(p_for.getLocation().col).c_str());
        recordError(p_for.getLocation().line, p_for.getLocation().col);
        symbol_mgr.popScope();
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
    symbol_mgr.popScope();
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    // 1. Push a new symbol table if this node forms a scope.
    auto symTab = symbol_mgr.currentSymTab();
    // 2. Insert the symbol into current symbol table if this node is related to
    //    declaration (ProgramNode, VariableNode, FunctionNode).
    // 3. Travere child nodes of this node.
    p_return.visitChildNodes(*this);
    // 4. Perform semantic analyses of this node.
    // The current context shouldn't be in the program or a procedure since their return type is void
    if (symTab->ctx_kind == ContextKind::Program || symTab->ctx_kind == ContextKind::Procedure) {
        fprintf(stderr, "<Error> Found in line %u, column %u: program/procedure should not return a value\n"
                        "    %s\n"
                        "    %s\n",
                        p_return.getLocation().line, p_return.getLocation().col,
                        getSourceLine(p_return.getLocation().line).c_str(),
                        getErrIndicator(p_return.getLocation().col).c_str());
        recordError(p_return.getLocation().line, p_return.getLocation().col);
        return;
    }
    // Skip the rest of semantic checks if there are any errors in the node of the expression (return value)
    if (hasErrorAt(p_return.expression->getLocation().line, p_return.expression->getLocation().col)) {
        recordError(p_return.getLocation().line, p_return.getLocation().col);
        return;
    }
    // The type of the result of the expression (return value) must be the same type as the return type of current function after appropriate type coercion
    if (!typeEq(p_return.expression->getType(), symTab->ctx_type)) {
        fprintf(stderr, "<Error> Found in line %u, column %u: return '%s' from a function with return type '%s'\n"
                        "    %s\n"
                        "    %s\n",
                        p_return.expression->getLocation().line, p_return.expression->getLocation().col,
                        p_return.expression->getType()->getTypeStr().c_str(),
                        symTab->ctx_type->getTypeStr().c_str(),
                        getSourceLine(p_return.expression->getLocation().line).c_str(),
                        getErrIndicator(p_return.expression->getLocation().col).c_str());
        recordError(p_return.expression->getLocation().line, p_return.expression->getLocation().col);
        recordError(p_return.getLocation().line, p_return.getLocation().col);
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
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
    else if (exist_symbol != nullptr && exist_symbol->getKind() == SymbolEntryKind::LoopVar) {
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

    if (t1->kind == TypeKind::Real && t2->kind == TypeKind::Integer) {
        return t1;
    }
    else if (t2->kind == TypeKind::Real && t1->kind == TypeKind::Integer) {
        return t2;
    }
    return nullptr;
}

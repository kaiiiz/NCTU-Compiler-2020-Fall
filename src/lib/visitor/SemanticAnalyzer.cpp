#include "visitor/SemanticAnalyzer.hpp"

#include <iostream>
#include <vector>

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
#include "sema/SemaErrHandler.hpp"

#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

static inline bool is_void(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Void; }
static inline bool is_real(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Real; }
static inline bool is_int(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Integer; }
static inline bool is_bool(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::Boolean; }
static inline bool is_str(std::shared_ptr<TypeStruct> t) { return t->kind == TypeKind::String; }

SemanticAnalyzer::SemanticAnalyzer(SymbolManager &symbol_mgr, std::vector<long> &line_head, std::string source_filename)
    : symbol_mgr(symbol_mgr), err_handler(line_head, source_filename) {}

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
                err_handler.declArrayIdxNotGt0(p_variable.getLocation(),
                                               p_variable.getNameStr());
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
    if (err_handler.hasErrorAt(p_print.expr->getLocation())) {
        err_handler.recordError(p_print.getLocation());
        return;
    }
    // The type of the expression (target) must be scalar type
    if (p_print.expr->getType()->isArray() || p_print.expr->getType()->kind == TypeKind::Void) {
        err_handler.printExprType(p_print.getLocation(), p_print.expr->getLocation());
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
    if (err_handler.hasErrorAt(lexpr->getLocation()) || err_handler.hasErrorAt(rexpr->getLocation())) {
        err_handler.recordError(p_bin_op.getLocation());
        return;
    }
    auto log_error = [this, &p_bin_op, &lexpr_type, &rexpr_type]() {
        err_handler.binOpInvalidOperand(p_bin_op.getLocation(),
                                        p_bin_op.getOPString(),
                                        lexpr_type->getTypeStr(),
                                        rexpr_type->getTypeStr());
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
    if (err_handler.hasErrorAt(p_un_op.expr->getLocation())) {
        err_handler.recordError(p_un_op.getLocation());
        return;
    }
    auto log_error = [this, &p_un_op, &expr_type]() {
        err_handler.unaryOpInvalidOperand(p_un_op.getLocation(),
                                          p_un_op.getOPString(),
                                          expr_type->getTypeStr());
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
        err_handler.useOfUndeclaredSymbol(p_func_invocation.getLocation(),
                                          p_func_invocation.getFuncName());
        return;
    }
    // The kind of symbol has to be function.
    else if (symbol->getKind() != SymbolEntryKind::Function) {
        err_handler.callNonFuncSymbol(p_func_invocation.getLocation(),
                                      p_func_invocation.getFuncName());
        return;
    }
    // Pointer cast to function symbol after checking symbol kind is function
    auto func_symbol = std::dynamic_pointer_cast<FunctionSymbolEntry>(symbol);
    // The number of arguments must be the same as one of the parameters.
    if (func_symbol->getParamType().size() != p_func_invocation.getExprs().size()) {
        err_handler.callArgsNumMismatch(p_func_invocation.getLocation(),
                                     p_func_invocation.getFuncName());
        return;
    }
    // Traverse arguments
    auto param_prototype = func_symbol->getParamType();
    auto arg_exprs = p_func_invocation.getExprs();
    for (uint64_t i = 0; i < param_prototype.size(); i++) {
        if (!typeEq(param_prototype[i], arg_exprs[i]->getType())) {
            err_handler.callArgsTypeMismatch(p_func_invocation.getLocation(),
                                             arg_exprs[i]->getLocation(),
                                             arg_exprs[i]->getType()->getTypeStr(),
                                             param_prototype[i]->getTypeStr());
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
        err_handler.useOfUndeclaredSymbol(p_variable_ref.getLocation(),
                                          p_variable_ref.getNameStr());
        return;
    }
    // The kind of symbol has to be a parameter, variable, loop_var, or constant.
    else if (symbol->getKind() != SymbolEntryKind::Parameter &&
             symbol->getKind() != SymbolEntryKind::Variable &&
             symbol->getKind() != SymbolEntryKind::LoopVar &&
             symbol->getKind() != SymbolEntryKind::Constant) {
        err_handler.useOfNonVarSymbol(p_variable_ref.getLocation(),
                                      p_variable_ref.getNameStr());
        return;
    }
    // Skip the rest of semantic checks if there are any errors in the node of the declaration of the refered symbol
    if (err_handler.hasErrorAt(symbol->getLocation())) {
        err_handler.recordError(p_variable_ref.getLocation());
        return;
    }
    // Each index of an array reference must be of the integer type.
    for (auto &e : p_variable_ref.getExprs()) {
        if (err_handler.hasErrorAt(e->getLocation())) {
            err_handler.recordError(p_variable_ref.getLocation());
            return;
        }
        if (e->getType()->kind != TypeKind::Integer) {
            err_handler.arrayRefIdxType(p_variable_ref.getLocation(), e->getLocation());
            return;
        }
    }
    // An over array subscript is forbidden, that is, the number of indices of an array reference cannot be greater than the one of dimensions in the declaration.
    if (symbol->getType()->dim.size() < p_variable_ref.getExprs().size()) {
        err_handler.arrayOverSubscript(p_variable_ref.getLocation(), p_variable_ref.getNameStr());
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
    if (err_handler.hasErrorAt(p_assignment.var_ref->getLocation())) {
        err_handler.recordError(p_assignment.getLocation());
        return;
    }
    // The type of the result of the variable reference cannot be an array type
    if (p_assignment.var_ref->getType()->isArray()) {
        err_handler.arrayAssignment(p_assignment.getLocation(),
                                    p_assignment.var_ref->getLocation());
        return;
    }
    // The variable reference cannot be a reference to a constant variable
    auto symbol = symTab->lookup(p_assignment.var_ref->getNameStr());
    if (symbol->getKind() == SymbolEntryKind::Constant) {
        err_handler.constAssignment(p_assignment.getLocation(),
                                    p_assignment.var_ref->getLocation(),
                                    p_assignment.var_ref->getNameStr());
        return;
    }
    // The variable reference cannot be a reference to a loop variable when the context is within a loop body
    if (symbol->getKind() == SymbolEntryKind::LoopVar && symTab->level > symbol->getLevel()) {
        err_handler.loopVarAssignment(p_assignment.getLocation(),
                                      p_assignment.var_ref->getLocation());
        return;
    }

    // Skip the rest of semantic checks if there are any errors in the node of the expression
    if (err_handler.hasErrorAt(p_assignment.expression->getLocation())) {
        err_handler.recordError(p_assignment.getLocation());
        return;
    }
    // The type of the result of the expression cannot be an array type
    if (p_assignment.expression->getType()->isArray()) {
        err_handler.arrayAssignment(p_assignment.getLocation(),
                                    p_assignment.expression->getLocation());
        return;
    }
    // The type of the variable reference (lvalue) must be the same as the one of the expression after appropriate type coercion
    else if (!typeEq(p_assignment.var_ref->getType(), p_assignment.expression->getType())) {
        err_handler.assignTypeMismatch(p_assignment.getLocation(),
                                       p_assignment.var_ref->getType()->getTypeStr(),
                                       p_assignment.expression->getType()->getTypeStr());
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
    if (err_handler.hasErrorAt(p_read.var_ref->getLocation())) {
        err_handler.recordError(p_read.getLocation());
        return;
    }
    // The type of the variable reference must be scalar type
    if (p_read.var_ref->getType()->isArray()) {
        err_handler.readVarRefType(p_read.getLocation(), p_read.var_ref->getLocation());
        return;
    }
    // The kind of symbol of the variable reference cannot be constant or loop_var
    auto symbol = symTab->lookup(p_read.var_ref->getNameStr());
    if (symbol->getKind() == SymbolEntryKind::Constant || symbol->getKind() == SymbolEntryKind::LoopVar) {
        err_handler.readVarRefNoConstOrLoopVar(p_read.getLocation(), p_read.var_ref->getLocation());
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
    if (err_handler.hasErrorAt(p_if.condition->getLocation())) {
        err_handler.recordError(p_if.getLocation());
        return;
    }
    // The type of the result of the expression (condition) must be boolean type
    if (p_if.condition->getType()->kind != TypeKind::Boolean) {
        err_handler.conditionType(p_if.getLocation(), p_if.condition->getLocation());
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
    if (err_handler.hasErrorAt(p_while.condition->getLocation())) {
        err_handler.recordError(p_while.getLocation());
        return;
    }
    // The type of the result of the expression (condition) must be boolean type
    if (p_while.condition->getType()->kind != TypeKind::Boolean) {
        err_handler.conditionType(p_while.getLocation(), p_while.condition->getLocation());
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
        err_handler.forIterOrder(p_for.getLocation());
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
        err_handler.returnInVoidCtx(p_return.getLocation());
        return;
    }
    // Skip the rest of semantic checks if there are any errors in the node of the expression (return value)
    if (err_handler.hasErrorAt(p_return.expression->getLocation())) {
        err_handler.recordError(p_return.getLocation());
        return;
    }
    // The type of the result of the expression (return value) must be the same type as the return type of current function after appropriate type coercion
    if (!typeEq(p_return.expression->getType(), symTab->ctx_type)) {
        err_handler.returnTypeMismatch(p_return.getLocation(),
                                       p_return.expression->getLocation(),
                                       p_return.expression->getType()->getTypeStr(),
                                       symTab->ctx_type->getTypeStr());
        return;
    }
    // 5. Pop the symbol table pushed at the 1st step.
}

bool SemanticAnalyzer::insertWithCheck(std::shared_ptr<SymbolTable> sym_tab,
                                       std::shared_ptr<SymbolEntry> symbol) {
    auto symbol_name = symbol->getNameStr();
    auto exist_symbol = sym_tab->lookup(symbol_name);
    // normal symbol redeclaration check
    // loop var redeclaration check
    if ((exist_symbol != nullptr && exist_symbol->getLevel() == symbol->getLevel()) ||
        (exist_symbol != nullptr && exist_symbol->getKind() == SymbolEntryKind::LoopVar)) {
        err_handler.symbolRedeclaration(symbol->getLocation(), symbol->getNameStr());
        return false;
    }
    sym_tab->insert(symbol);
    return true;
}

bool SemanticAnalyzer::hasError() {
    return err_handler.hasError();
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

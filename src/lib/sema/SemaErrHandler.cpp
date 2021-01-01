#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>

#include "sema/SemaErrHandler.hpp"
#include "AST/ast.hpp"

#define PERR(loc, fmt, ...) \
    fprintf(stderr, "<Error> Found in line %u, column %u: " fmt "    %s\n    %s\n", \
            loc.line, loc.col, \
            ##__VA_ARGS__, \
            getSourceLine(loc.line).c_str(), \
            getErrIndicator(loc.col).c_str())

SemaErrHandler::SemaErrHandler(std::vector<long> &line_head, std::string source_filename)
    : line_head(line_head), source_filename(source_filename) {}

void SemaErrHandler::declArrayIdxNotGt0(const Location &var_loc, std::string var_name) {
    PERR(var_loc,
         "'%s' declared as an array with an index that is not greater than 0\n",
         var_name.c_str());
    recordError(var_loc);
}

void SemaErrHandler::printExprType(const Location &print_loc, const Location &expr_loc) {
    PERR(expr_loc,
         "expression of print statement must be scalar type\n");
    recordError(expr_loc);
    recordError(print_loc);
}

void SemaErrHandler::binOpInvalidOperand(const Location &op_loc, std::string op,
                                         std::string ltype, std::string rtype) {
    PERR(op_loc,
         "invalid operands to binary operator '%s' ('%s' and '%s')\n",
         op.c_str(), ltype.c_str(), rtype.c_str());
    recordError(op_loc);
}

void SemaErrHandler::unaryOpInvalidOperand(const Location &op_loc, std::string op, std::string expr_type) {
    PERR(op_loc,
         "invalid operand to unary operator '%s' ('%s')\n",
         op.c_str(), expr_type.c_str());
    recordError(op_loc);
}

void SemaErrHandler::useOfUndeclaredSymbol(const Location &var_loc, std::string var_name) {
    PERR(var_loc,
         "use of undeclared symbol '%s'\n",
         var_name.c_str());
    recordError(var_loc);
}

void SemaErrHandler::callNonFuncSymbol(const Location &call_loc, std::string call_name) {
    PERR(call_loc,
         "call of non-function symbol '%s'\n",
         call_name.c_str());
    recordError(call_loc);
}

void SemaErrHandler::callArgsNumMismatch(const Location &call_loc, std::string call_name) {
    PERR(call_loc,
         "too few/much arguments provided for function '%s'\n",
         call_name.c_str());
    recordError(call_loc);
}

void SemaErrHandler::callArgsTypeMismatch(const Location &call_loc, const Location &arg_loc,
                                          std::string arg_type, std::string param_type) {
    PERR(arg_loc,
         "incompatible type passing '%s' to parameter of type '%s'\n",
         arg_type.c_str(), param_type.c_str());
    recordError(arg_loc);
    recordError(call_loc);
}

void SemaErrHandler::useOfNonVarSymbol(const Location &var_loc, std::string var_name) {
    PERR(var_loc,
         "use of non-variable symbol '%s'\n",
         var_name.c_str());
    recordError(var_loc);
}

void SemaErrHandler::arrayRefIdxType(const Location &var_ref_loc, const Location &expr_loc) {
    PERR(expr_loc,
         "index of array reference must be an integer\n");
    recordError(expr_loc);
    recordError(var_ref_loc);
}

void SemaErrHandler::arrayOverSubscript(const Location &var_ref_loc, std::string var_name) {
    PERR(var_ref_loc,
         "there is an over array subscript on '%s'\n",
         var_name.c_str());
    recordError(var_ref_loc);
}

void SemaErrHandler::arrayAssignment(const Location &assign_loc, const Location &var_ref_loc) {
    PERR(var_ref_loc,
         "array assignment is not allowed\n");
    recordError(var_ref_loc);
    recordError(assign_loc);
}

void SemaErrHandler::constAssignment(const Location &assign_loc, const Location &var_ref_loc,
                                     std::string var_name) {
    PERR(var_ref_loc,
         "cannot assign to variable '%s' which is a constant\n",
         var_name.c_str());
    recordError(var_ref_loc);
    recordError(assign_loc);
}

void SemaErrHandler::loopVarAssignment(const Location &assign_loc, const Location &var_ref_loc) {
    PERR(var_ref_loc,
         "the value of loop variable cannot be modified inside the loop body\n");
    recordError(var_ref_loc);
    recordError(assign_loc);
}

void SemaErrHandler::assignTypeMismatch(const Location &assign_loc,
                                        std::string var_type,std::string expr_type) {
    PERR(assign_loc,
         "assigning to '%s' from incompatible type '%s'\n",
         var_type.c_str(), expr_type.c_str());
    recordError(assign_loc);
}

void SemaErrHandler::readVarRefType(const Location &read_loc, const Location &var_ref_loc) {
    PERR(var_ref_loc,
         "variable reference of read statement must be scalar type\n");
    recordError(var_ref_loc);
    recordError(read_loc);
}

void SemaErrHandler::readVarRefNoConstOrLoopVar(const Location &read_loc, const Location &var_ref_loc) {
    PERR(var_ref_loc,
         "variable reference of read statement cannot be a constant or loop variable\n");
    recordError(var_ref_loc);
    recordError(read_loc);
}

void SemaErrHandler::conditionType(const Location &stmt_loc, const Location &cond_loc) {
    PERR(cond_loc,
         "the expression of condition must be boolean type\n");
    recordError(cond_loc);
    recordError(stmt_loc);
}

void SemaErrHandler::forIterOrder(const Location &for_loc) {
    PERR(for_loc,
         "the lower bound and upper bound of iteration count must be in the incremental order\n");
    recordError(for_loc);
}

void SemaErrHandler::returnInVoidCtx(const Location &ret_loc) {
    PERR(ret_loc,
         "program/procedure should not return a value\n");
    recordError(ret_loc);
}


void SemaErrHandler::returnTypeMismatch(const Location &ret_loc, const Location &expr_loc,
                                        std::string expr_ret_type, std::string func_ret_type) {
    PERR(expr_loc,
         "return '%s' from a function with return type '%s'\n",
         expr_ret_type.c_str(), func_ret_type.c_str());
    recordError(expr_loc);
    recordError(ret_loc);
}

void SemaErrHandler::symbolRedeclaration(const Location &symbol_loc, std::string symbol_name) {
    PERR(symbol_loc,
         "symbol '%s' is redeclared\n",
         symbol_name.c_str());
    recordError(symbol_loc);
}

std::string SemaErrHandler::getSourceLine(long lineno) {
    std::ifstream source_file(source_filename);
    std::string line;
    if (!source_file.good()) {
        std::cerr << "[SemaErrHandler] File stream is not good" << std::endl;
        exit(EXIT_FAILURE);
    }
    source_file.seekg(line_head[lineno - 1]);
    std::getline(source_file, line);
    return line;
}

std::string SemaErrHandler::getErrIndicator(long col) {
    std::string err_indicator;
    for (int i = 0; i < col - 1; i++) err_indicator += " ";
    err_indicator += "^";
    return err_indicator;
}

void SemaErrHandler::recordError(long lineno, long col) {
    error_list[lineno].push_back(col);
}

void SemaErrHandler::recordError(const Location &loc) {
    recordError(loc.line, loc.col);
}

bool SemaErrHandler::hasErrorAt(long lineno, long col) {
    if (error_list.find(lineno) == error_list.end()) { return false; }
    auto &err_in_line = error_list[lineno];
    return std::find(err_in_line.begin(), err_in_line.end(), col) != err_in_line.end();
}

bool SemaErrHandler::hasErrorAt(const Location &loc) {
    return hasErrorAt(loc.line, loc.col);
}

bool SemaErrHandler::hasError() {
    return !error_list.empty();
}

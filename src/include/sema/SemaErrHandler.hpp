#ifndef __SEMA_ERR_HANDLER__
#define __SEMA_ERR_HANDLER__

#include <string>
#include <vector>
#include <unordered_map>

struct Location;

class SemaErrHandler {
   public:
    SemaErrHandler(std::vector<long> &line_head, std::string source_filename);

    bool hasError();
    bool hasErrorAt(long lineno, long col);
    bool hasErrorAt(const Location &loc);
    void recordError(long lineno, long col);
    void recordError(const Location &loc);

    void declArrayIdxNotGt0(const Location &var_loc, std::string var_name);
    void printExprType(const Location &print_loc, const Location &expr_loc);
    void binOpInvalidOperand(const Location &op_loc, std::string op, std::string ltype, std::string rtype);
    void unaryOpInvalidOperand(const Location &op_loc, std::string op, std::string expr_type);
    void useOfUndeclaredSymbol(const Location &var_loc, std::string var_name);
    void callNonFuncSymbol(const Location &call_loc, std::string call_name);
    void callArgsNumMismatch(const Location &call_loc, std::string call_name);
    void callArgsTypeMismatch(const Location &call_loc, const Location &arg_loc,
                              std::string arg_type, std::string param_type);
    void useOfNonVarSymbol(const Location &var_loc, std::string var_name);
    void arrayRefIdxType(const Location &var_ref_loc, const Location &expr_loc);
    void arrayOverSubscript(const Location &var_ref_loc, std::string var_name);
    void arrayAssignment(const Location &assign_loc, const Location &var_ref_loc);
    void constAssignment(const Location &assign_loc, const Location &var_ref_loc, std::string var_name);
    void loopVarAssignment(const Location &assign_loc, const Location &var_ref_loc);
    void assignTypeMismatch(const Location &assign_loc, std::string var_type, std::string expr_type);
    void readVarRefType(const Location &read_loc, const Location &var_ref_loc);
    void readVarRefNoConstOrLoopVar(const Location &read_loc, const Location &var_ref_loc);
    void conditionType(const Location &stmt_loc, const Location &cond_loc);
    void forIterOrder(const Location &for_loc);
    void returnInVoidCtx(const Location &ret_loc);
    void returnTypeMismatch(const Location &ret_loc, const Location &expr_loc,
                            std::string expr_ret_type, std::string func_ret_type);
    void symbolRedeclaration(const Location &symbol_loc, std::string symbol_name);

   private:
    std::string getSourceLine(long lineno);
    std::string getErrIndicator(long col);

    std::vector<long> &line_head;
    std::string source_filename;
    std::unordered_map<long, std::vector<long>> error_list; // [lineno] = list(col)
};

#endif
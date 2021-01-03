#ifndef __CODE_GENERATOR_H
#define __CODE_GENERATOR_H

#include <fstream>
#include "sema/SemaInclude.hpp"
#include "visitor/AstNodeVisitor.hpp"

enum class BinaryOP;
enum class UnaryOP;
class ExpressionBase;

class CodeGenerator : public AstNodeVisitor {
   public:
    CodeGenerator(SymbolManager &symbol_mgr, const std::string &in_file_name, const std::string &out_file_path);
    ~CodeGenerator();

    void dumpInstrs(const char *format, ...);

    void visit(ProgramNode &p_program) override;
    void visit(DeclNode &p_decl) override;
    void visit(VariableNode &p_variable) override;
    void visit(ConstantValueNode &p_constant_value) override;
    void visit(FunctionNode &p_function) override;
    void visit(CompoundStatementNode &p_compound_statement) override;
    void visit(PrintNode &p_print) override;
    void visit(BinaryOperatorNode &p_bin_op) override;
    void visit(UnaryOperatorNode &p_un_op) override;
    void visit(FunctionInvocationNode &p_func_invocation) override;
    void visit(FunctionCallNode &p_func_call);
    void visit(VariableReferenceNode &p_variable_ref) override;
    void visit(AssignmentNode &p_assignment) override;
    void visit(ReadNode &p_read) override;
    void visit(IfNode &p_if) override;
    void visit(WhileNode &p_while) override;
    void visit(ForNode &p_for) override;
    void visit(ReturnNode &p_return) override;

   private:
    void genGlobalVarAddrLoad(std::string var_name,
                              const std::shared_ptr<TypeStruct> decl_type,
                              const std::vector<std::shared_ptr<ExpressionBase>> exprs);
    void genGlobalVarLoad(std::string var_name,
                          const std::shared_ptr<TypeStruct> decl_type,
                          const std::vector<std::shared_ptr<ExpressionBase>> exprs);
    void genLocalVarAddrLoad(int fp_offset,
                             SymbolEntryKind decl_kind,
                             const std::shared_ptr<TypeStruct> decl_type,
                             const std::vector<std::shared_ptr<ExpressionBase>> exprs);
    void genLocalVarLoad(int fp_offset,
                         SymbolEntryKind decl_kind,
                         const std::shared_ptr<TypeStruct> decl_type,
                         const std::vector<std::shared_ptr<ExpressionBase>> exprs);
    void genStackTopAddrToValue();
    void genPushGlobalVarAddr(std::string var_name);
    void genPushLocalVarAddr(int fp_offset);
    void genCaclArrayOffset(const std::shared_ptr<TypeStruct> decl_type,
                            const std::vector<std::shared_ptr<ExpressionBase>> exprs);
    void genStackTopLeftShift(int bits);

    void genFunctionPrologue(std::string func_name);
    void genFunctionEpilogue(std::string func_name);
    void genGlobalVarDecl(std::string var_name, int bytes, int align);
    void genGlobalVarConst(std::string var_name, std::string val_str);
    void genParamLoad(int param_num, int fp_offset);
    void genParamStore(int param_num);
    void genReturn();
    void genFuncCall(std::string func_name);
    void genReturnValStore();
    void genConstStore(std::string val);
    void genAssign();
    void genPrint();
    void genRead();
    void genBinaryOperation(BinaryOP op);
    void genUnaryOperation(UnaryOP op);
    void genIfFalseBranch(int label);
    void genLabel(int label);
    void genJump(int label);
    void genStackPop(int bytes);

    int getLabel();
    const std::string &in_file_name;
    std::ofstream output_file;
    SymbolManager &symbol_mgr;
    int label = 0;
};

#endif

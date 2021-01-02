#ifndef __CODE_GENERATOR_H
#define __CODE_GENERATOR_H

#include <fstream>
#include "sema/SemaInclude.hpp"
#include "visitor/AstNodeVisitor.hpp"

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
    void genFunctionPrologue(std::string func_name);
    void genFunctionEpilogue(std::string func_name);
    void genGlobalVarDecl(std::string var_name, int size, int align);
    void genGlobalVarConst(std::string var_name, std::string val_str);
    void genGlobalVarLoad(std::string var_name);
    void genLocalVarLoad(std::shared_ptr<SymbolEntry> symbol);
    void genConstLoad(std::string val);
    void genAssign();

    const std::string &in_file_name;
    std::ofstream output_file;
    SymbolManager &symbol_mgr;
};

#endif
#ifndef __SEMA_SEMANTIC_ANALYZER_H
#define __SEMA_SEMANTIC_ANALYZER_H

#include <vector>
#include <string>
#include <unordered_map>

#include "visitor/AstNodeVisitor.hpp"
#include "sema/SymbolManager.hpp"
#include "type/struct.hpp"

class SymbolEntry;

class SemanticAnalyzer : public AstNodeVisitor {
  public:
    SemanticAnalyzer(SymbolManager& symbol_mgr, std::vector<long> &line_head, std::string source_filename);
    ~SemanticAnalyzer() = default;

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
    void visit(VariableReferenceNode &p_variable_ref) override;
    void visit(AssignmentNode &p_assignment) override;
    void visit(ReadNode &p_read) override;
    void visit(IfNode &p_if) override;
    void visit(WhileNode &p_while) override;
    void visit(ForNode &p_for) override;
    void visit(ReturnNode &p_return) override;
    void dumpSymTab();
    bool hasError();

  private:
    bool insertWithCheck(std::shared_ptr<SymbolTable> sym_tab, std::shared_ptr<SymbolEntry> symbol);
    std::string getSourceLine(long lineno);
    std::string getErrIndicator(long col);
    void recordError(long lineno, long col);
    bool hasErrorAt(long lineno, long col);
    bool typeEq(std::shared_ptr<TypeStruct> t1, std::shared_ptr<TypeStruct> t2);
    std::shared_ptr<TypeStruct> coerce(std::shared_ptr<TypeStruct> t1, std::shared_ptr<TypeStruct> t2);

    SymbolManager& symbol_mgr;
    std::vector<long> &line_head;
    std::string source_filename;
    std::unordered_map<long, std::vector<long>> error_list; // [lineno] = list(col)
};

#endif

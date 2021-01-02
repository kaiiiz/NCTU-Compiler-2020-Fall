#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include <vector>
#include <memory>

#include "AST/ast.hpp"
#include "AST/base/AstNodeWithSymTab.hpp"

class AstDumper;
class DeclNode;
class FunctionNode;
class CompoundStatementNode;
class TypeStruct;

class ProgramNode : public AstNode, public AstNodeWithSymTab {
  public:
    ProgramNode(const uint32_t line, const uint32_t col,
                const std::string p_name, std::shared_ptr<TypeStruct> ret_type,
                const std::vector<std::shared_ptr<DeclNode>> decl_list,
                const std::vector<std::shared_ptr<FunctionNode>> func_list,
                const std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~ProgramNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    std::string getProgramName();
    std::shared_ptr<TypeStruct> getReturnType();

  private:
    const std::string name;
    std::shared_ptr<TypeStruct> ret_type;
    const std::vector<std::shared_ptr<DeclNode>> decl_list;
    const std::vector<std::shared_ptr<FunctionNode>> func_list;
    const std::shared_ptr<CompoundStatementNode> compound_stmt;
};

#endif

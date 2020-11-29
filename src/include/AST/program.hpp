#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include <vector>
#include <memory>

#include "AST/ast.hpp"

class AstDumper;
class DeclNode;
class FunctionNode;
class CompoundStatementNode;

class ProgramNode : public AstNode {
  public:
    ProgramNode(const uint32_t line, const uint32_t col,
                const std::string p_name, const std::string ret_type,
                const std::vector<std::shared_ptr<DeclNode>> decl_list,
                const std::vector<std::shared_ptr<FunctionNode>> func_list,
                const std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~ProgramNode() = default;

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;
    const char *getProgramName();

  private:
    const std::string name;
    const std::string ret_type;
    const std::vector<std::shared_ptr<DeclNode>> decl_list;
    const std::vector<std::shared_ptr<FunctionNode>> func_list;
    const std::shared_ptr<CompoundStatementNode> compound_stmt;
};

#endif

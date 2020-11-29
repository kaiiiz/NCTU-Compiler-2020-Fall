#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include <vector>
#include <memory>

#include "AST/ast.hpp"

class AstDumper;

class ProgramNode : public AstNode {
  public:
    ProgramNode(const uint32_t line, const uint32_t col,
                const std::string p_name, const std::string ret_type,
                const std::vector<std::shared_ptr<AstNode>> decl_list,
                const std::vector<std::shared_ptr<AstNode>> func_list,
                const std::shared_ptr<AstNode> compound_stmt);
    ~ProgramNode() = default;

    void dump(AstDumper &dp) override;
    const char *getProgramName();

  private:
    const std::string name;
    const std::string ret_type;
    const std::vector<std::shared_ptr<AstNode>> decl_list;
    const std::vector<std::shared_ptr<AstNode>> func_list;
    const std::shared_ptr<AstNode> compound_stmt;
};

#endif

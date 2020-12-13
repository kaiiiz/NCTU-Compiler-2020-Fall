#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class DeclNode;
class AssignmentNode;
class ConstantValueNode;
class CompoundStatementNode;

class ForNode : public StatementBase {
  public:
    ForNode(const uint32_t line, const uint32_t col,
            std::shared_ptr<DeclNode> declaration,
            std::shared_ptr<AssignmentNode> assignment,
            std::shared_ptr<ConstantValueNode> condition,
            std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~ForNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::shared_ptr<DeclNode> declaration;
    std::shared_ptr<AssignmentNode> assignment;
    std::shared_ptr<ConstantValueNode> condition;
    std::shared_ptr<CompoundStatementNode> compound_stmt;
};

#endif

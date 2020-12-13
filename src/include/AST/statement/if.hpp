#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;
class CompoundStatementNode;

class IfNode : public StatementBase {
  public:
    IfNode(const uint32_t line, const uint32_t col,
           std::shared_ptr<ExpressionBase> condition,
           std::shared_ptr<CompoundStatementNode> body,
           std::shared_ptr<CompoundStatementNode> body_else);
    ~IfNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::shared_ptr<ExpressionBase> condition;
    std::shared_ptr<CompoundStatementNode> body;
    std::shared_ptr<CompoundStatementNode> body_else;
};

#endif

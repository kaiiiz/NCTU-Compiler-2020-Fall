#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;
class CompoundStatementNode;

class IfNode : public StatementBase {
  public:
    IfNode(const uint32_t line, const uint32_t col,
           const std::shared_ptr<ExpressionBase> condition,
           const std::shared_ptr<CompoundStatementNode> body,
           const std::shared_ptr<CompoundStatementNode> body_else);
    ~IfNode() = default;

    const std::shared_ptr<ExpressionBase> condition;
    const std::shared_ptr<CompoundStatementNode> body;
    const std::shared_ptr<CompoundStatementNode> body_else;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

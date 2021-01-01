#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;
class CompoundStatementNode;

class WhileNode : public StatementBase {
  public:
    WhileNode(const uint32_t line, const uint32_t col,
              const std::shared_ptr<ExpressionBase> condition,
              const std::shared_ptr<CompoundStatementNode> body);
    ~WhileNode() = default;

    const std::shared_ptr<ExpressionBase> condition;
    const std::shared_ptr<CompoundStatementNode> body;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

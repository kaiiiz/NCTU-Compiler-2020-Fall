#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;

class ReturnNode : public StatementBase {
  public:
    ReturnNode(const uint32_t line, const uint32_t col,
               const std::shared_ptr<ExpressionBase> expression);
    ~ReturnNode() = default;

    const std::shared_ptr<ExpressionBase> expression;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

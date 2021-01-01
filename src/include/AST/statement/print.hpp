#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;

class PrintNode : public StatementBase {
  public:
    PrintNode(const uint32_t line, const uint32_t col, const std::shared_ptr<ExpressionBase> expr);
    ~PrintNode() = default;

    const std::shared_ptr<ExpressionBase> expr;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

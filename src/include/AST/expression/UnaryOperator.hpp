#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/base/ExpressionBase.hpp"

enum class UnaryOP {
    MINUS,
    NOT,
};

class UnaryOperatorNode : public ExpressionBase {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col, UnaryOP op,
                       std::shared_ptr<ExpressionBase> expr);
    ~UnaryOperatorNode() = default;

    std::string getOPString();
    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    UnaryOP op;
    std::shared_ptr<ExpressionBase> expr;
};

#endif

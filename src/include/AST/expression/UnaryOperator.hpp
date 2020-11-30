#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/base/ExpressionBase.hpp"

class UnaryOperatorNode : public ExpressionBase {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col
                      /* TODO: operator, expression */);
    ~UnaryOperatorNode() = default;

  private:
    // TODO: operator, expression
};

#endif

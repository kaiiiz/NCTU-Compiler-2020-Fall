#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/base/ExpressionBase.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionBase {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col
                       /* TODO: operator, expressions */);
    ~BinaryOperatorNode() = default;

  private:
    // TODO: operator, expressions
};

#endif

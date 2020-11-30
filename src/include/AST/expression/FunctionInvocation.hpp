#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/base/ExpressionBase.hpp"

class FunctionInvocationNode : public ExpressionBase {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col
                           /* TODO: function name, expressions */);
    ~FunctionInvocationNode() = default;

  private:
    // TODO: function name, expressions
};

#endif

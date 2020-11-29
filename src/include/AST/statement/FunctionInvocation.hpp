#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/statement/statement.hpp"

class FunctionInvocationNode : public StatementNode {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col
                           /* TODO: function name, expressions */);
    ~FunctionInvocationNode() = default;

    const char *getNameCString() const;

  private:
    // TODO: function name, expressions
};

#endif

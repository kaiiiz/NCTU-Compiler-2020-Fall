#ifndef __AST_FUNCTION_CALL_NODE_H
#define __AST_FUNCTION_CALL_NODE_H

#include "AST/base/StatementBase.hpp"

class FunctionCallNode : public StatementBase {
  public:
    FunctionCallNode(const uint32_t line, const uint32_t col
                           /* TODO: function name, expressions */);
    ~FunctionCallNode() = default;

  private:
    // TODO: function name, expressions
};

#endif

#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/base/StatementBase.hpp"

class IfNode : public StatementBase {
  public:
    IfNode(const uint32_t line, const uint32_t col
           /* TODO: expression, compound statement, compound statement */);
    ~IfNode() = default;

  private:
    // TODO: expression, compound statement, compound statement
};

#endif

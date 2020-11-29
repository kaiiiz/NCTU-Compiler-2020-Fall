#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/statement/statement.hpp"

class IfNode : public StatementNode {
  public:
    IfNode(const uint32_t line, const uint32_t col
           /* TODO: expression, compound statement, compound statement */);
    ~IfNode() = default;

  private:
    // TODO: expression, compound statement, compound statement
};

#endif

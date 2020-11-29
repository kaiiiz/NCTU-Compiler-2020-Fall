#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/statement/statement.hpp"

class WhileNode : public StatementNode {
  public:
    WhileNode(const uint32_t line, const uint32_t col
              /* TODO: expression, compound statement */);
    ~WhileNode() = default;

  private:
    // TODO: expression, compound statement
};

#endif

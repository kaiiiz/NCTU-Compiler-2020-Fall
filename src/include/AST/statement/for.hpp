#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/statement/statement.hpp"

class ForNode : public StatementNode {
  public:
    ForNode(const uint32_t line, const uint32_t col
            /* TODO: declaration, assignment, expression,
             *       compound statement */);
    ~ForNode() = default;

  private:
    // TODO: declaration, assignment, expression, compound statement
};

#endif

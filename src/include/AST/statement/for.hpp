#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/base/StatementBase.hpp"

class ForNode : public StatementBase {
  public:
    ForNode(const uint32_t line, const uint32_t col
            /* TODO: declaration, assignment, expression,
             *       compound statement */);
    ~ForNode() = default;

  private:
    // TODO: declaration, assignment, expression, compound statement
};

#endif

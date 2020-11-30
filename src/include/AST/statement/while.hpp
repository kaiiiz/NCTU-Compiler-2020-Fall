#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/base/StatementBase.hpp"

class WhileNode : public StatementBase {
  public:
    WhileNode(const uint32_t line, const uint32_t col
              /* TODO: expression, compound statement */);
    ~WhileNode() = default;

  private:
    // TODO: expression, compound statement
};

#endif

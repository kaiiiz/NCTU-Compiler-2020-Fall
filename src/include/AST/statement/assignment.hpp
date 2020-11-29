#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/statement/statement.hpp"

class AssignmentNode : public StatementNode {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col
                   /* TODO: variable reference, expression */);
    ~AssignmentNode() = default;

  private:
    // TODO: variable reference, expression
};

#endif

#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/base/StatementBase.hpp"

class AssignmentNode : public StatementBase {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col
                   /* TODO: variable reference, expression */);
    ~AssignmentNode() = default;

  private:
    // TODO: variable reference, expression
};

#endif

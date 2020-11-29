#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/statement/statement.hpp"

class PrintNode : public StatementNode {
  public:
    PrintNode(const uint32_t line, const uint32_t col
              /* TODO: expression */);
    ~PrintNode() = default;

  private:
    // TODO: expression
};

#endif

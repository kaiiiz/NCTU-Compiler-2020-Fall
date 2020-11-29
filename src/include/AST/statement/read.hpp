#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/statement/statement.hpp"

class ReadNode : public StatementNode {
  public:
    ReadNode(const uint32_t line, const uint32_t col
             /* TODO: variable reference */);
    ~ReadNode() = default;

  private:
    // TODO: variable reference
};

#endif

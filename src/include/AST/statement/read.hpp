#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/base/StatementBase.hpp"

class ReadNode : public StatementBase {
  public:
    ReadNode(const uint32_t line, const uint32_t col
             /* TODO: variable reference */);
    ~ReadNode() = default;

  private:
    // TODO: variable reference
};

#endif

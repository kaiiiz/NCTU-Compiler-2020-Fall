#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/base/StatementBase.hpp"

class ReturnNode : public StatementBase {
  public:
    ReturnNode(const uint32_t line, const uint32_t col
               /* TODO: expression */);
    ~ReturnNode() = default;

  private:
    // TODO: expression
};

#endif

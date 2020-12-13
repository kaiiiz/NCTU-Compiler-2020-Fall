#ifndef __AST_STATEMENT_NODE_H
#define __AST_STATEMENT_NODE_H

#include "AST/ast.hpp"

class StatementBase : public AstNode {
  public:
    StatementBase(const uint32_t line, const uint32_t col);
    ~StatementBase();
};

#endif

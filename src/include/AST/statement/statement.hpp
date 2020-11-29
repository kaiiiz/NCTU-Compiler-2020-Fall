#ifndef __AST_STATEMENT_NODE_H
#define __AST_STATEMENT_NODE_H

#include "AST/ast.hpp"

class StatementNode : public AstNode {
  public:
    StatementNode(const uint32_t line, const uint32_t col);
    ~StatementNode();
};

#endif

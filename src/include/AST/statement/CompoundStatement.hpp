#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/statement/statement.hpp"

class CompoundStatementNode : public StatementNode {
  public:
    CompoundStatementNode(const uint32_t line, const uint32_t col
                          /* TODO: declarations, statements */);
    ~CompoundStatementNode() = default;

  private:
    // TODO: declarations, statements
};

#endif

#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include <vector>
#include <memory>

#include "AST/statement/statement.hpp"

class DeclNode;

class CompoundStatementNode : public StatementNode {
  public:
    CompoundStatementNode(const uint32_t line, const uint32_t col,
                          std::vector<std::shared_ptr<DeclNode>> declarations,
                          std::vector<std::shared_ptr<StatementNode>> statements);
    ~CompoundStatementNode() = default;

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::vector<std::shared_ptr<DeclNode>> declarations;
    std::vector<std::shared_ptr<StatementNode>> statements;
};

#endif

#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;
class CompoundStatementNode;

class WhileNode : public StatementBase {
  public:
    WhileNode(const uint32_t line, const uint32_t col,
              std::shared_ptr<ExpressionBase> condition,
              std::shared_ptr<CompoundStatementNode> body);
    ~WhileNode() = default;

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::shared_ptr<ExpressionBase> condition;
    std::shared_ptr<CompoundStatementNode> body;
};

#endif

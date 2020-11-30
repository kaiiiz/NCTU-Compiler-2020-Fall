#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionNode;

class PrintNode : public StatementBase {
  public:
    PrintNode(const uint32_t line, const uint32_t col, std::shared_ptr<ExpressionNode> expr);
    ~PrintNode() = default;

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::shared_ptr<ExpressionNode> expr;
};

#endif

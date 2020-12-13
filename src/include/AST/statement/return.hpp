#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class ExpressionBase;

class ReturnNode : public StatementBase {
  public:
    ReturnNode(const uint32_t line, const uint32_t col,
               std::shared_ptr<ExpressionBase> expression);
    ~ReturnNode() = default;

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::shared_ptr<ExpressionBase> expression;
};

#endif

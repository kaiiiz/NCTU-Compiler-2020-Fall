#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/base/ExpressionBase.hpp"

enum class UnaryOP {
    MINUS,
    NOT,
};

class UnaryOperatorNode : public ExpressionBase {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col, UnaryOP op,
                       std::shared_ptr<ExpressionBase> expr);
    ~UnaryOperatorNode() = default;

    std::string getOPString();
    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    UnaryOP op;
    std::shared_ptr<ExpressionBase> expr;
};

#endif

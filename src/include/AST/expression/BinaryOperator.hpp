#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"

enum class BinaryOP {
    MULTIPLY,
    DIVIDE,
    MOD,
    PLUS,
    MINUS,
    LESS,
    LESS_OR_EQUAL,
    GREATER,
    GREATER_OR_EQUAL,
    EQUAL,
    NOT_EQUAL,
    AND,
    OR,
};

class BinaryOperatorNode : public ExpressionBase {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col, BinaryOP op,
                       std::shared_ptr<ExpressionBase> lexpr,
                       std::shared_ptr<ExpressionBase> rexpr);
    ~BinaryOperatorNode() = default;

    std::string getOPString();
    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    BinaryOP op;
    std::shared_ptr<ExpressionBase> lexpr;
    std::shared_ptr<ExpressionBase> rexpr;
};

#endif

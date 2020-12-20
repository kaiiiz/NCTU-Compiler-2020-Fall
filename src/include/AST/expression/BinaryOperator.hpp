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

static inline std::string getBinaryOPStr(BinaryOP op) {
    switch (op) {
        case BinaryOP::PLUS:
            return "+";
        case BinaryOP::MINUS:
            return "-";
        case BinaryOP::MULTIPLY:
            return "*";
        case BinaryOP::DIVIDE:
            return "/";
        case BinaryOP::MOD:
            return "mod";
        case BinaryOP::LESS:
            return "<";
        case BinaryOP::LESS_OR_EQUAL:
            return "<=";
        case BinaryOP::NOT_EQUAL:
            return "<>";
        case BinaryOP::GREATER_OR_EQUAL:
            return ">=";
        case BinaryOP::GREATER:
            return ">";
        case BinaryOP::EQUAL:
            return "=";
        case BinaryOP::AND:
            return "and";
        case BinaryOP::OR:
            return "or";
        default:
            return "<not handled bin op>";
    }
}

class BinaryOperatorNode : public ExpressionBase {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col, const BinaryOP op,
                       const std::shared_ptr<ExpressionBase> lexpr,
                       const std::shared_ptr<ExpressionBase> rexpr);
    ~BinaryOperatorNode() = default;

    const BinaryOP op;
    const std::shared_ptr<ExpressionBase> lexpr;
    const std::shared_ptr<ExpressionBase> rexpr;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/base/ExpressionBase.hpp"

enum class UnaryOP {
    MINUS,
    NOT,
};

static inline std::string getUnaryOPStr(UnaryOP op) {
    switch (op) {
        case UnaryOP::NOT:
            return "not";
        case UnaryOP::MINUS:
            return "neg";
        default:
            return "<not handled unary op>";
    }
}

class UnaryOperatorNode : public ExpressionBase {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col, const UnaryOP op,
                      const std::shared_ptr<ExpressionBase> expr);
    ~UnaryOperatorNode() = default;

    const UnaryOP op;
    const std::shared_ptr<ExpressionBase> expr;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

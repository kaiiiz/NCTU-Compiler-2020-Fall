#include <memory>
#include "AST/expression/UnaryOperator.hpp"
#include "visitor/AstDumper.hpp"

UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col, const UnaryOP op,
                                     const std::shared_ptr<ExpressionBase> expr)
    : ExpressionBase{line, col}, op(op), expr(expr) {}

void UnaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expr->accept(p_visitor);
}

#include "AST/expression/BinaryOperator.hpp"
#include "visitor/AstDumper.hpp"

BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col, const BinaryOP op,
                                       const std::shared_ptr<ExpressionBase> lexpr,
                                       const std::shared_ptr<ExpressionBase> rexpr)
    : ExpressionBase{line, col}, op(op), lexpr(lexpr), rexpr(rexpr) {}

void BinaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    lexpr->accept(p_visitor);
    rexpr->accept(p_visitor);
}

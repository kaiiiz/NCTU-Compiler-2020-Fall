#include <memory>
#include "AST/expression/UnaryOperator.hpp"
#include "AST/AstDumper.hpp"

UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col, UnaryOP op,
                                     std::shared_ptr<ExpressionBase> expr)
    : ExpressionBase{line, col}, op(op), expr(expr) {}

std::string UnaryOperatorNode::getOPString() {
    switch (op) {
        case UnaryOP::NOT:
            return "not";
        case UnaryOP::MINUS:
            return "neg";
        default:
            return "<not handled>";
    }
}

void UnaryOperatorNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void UnaryOperatorNode::dumpChildNodes(AstDumper &dp) {
    expr->dump(dp);
}

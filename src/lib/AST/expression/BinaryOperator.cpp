#include "AST/expression/BinaryOperator.hpp"
#include "AST/AstDumper.hpp"

BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col, BinaryOP op,
                                       std::shared_ptr<ExpressionBase> lexpr,
                                       std::shared_ptr<ExpressionBase> rexpr)
    : ExpressionBase{line, col}, op(op), lexpr(lexpr), rexpr(rexpr) {}

std::string BinaryOperatorNode::getOPString() {
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
            return "<not handled>";
    }
}

void BinaryOperatorNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void BinaryOperatorNode::dumpChildNodes(AstDumper &dp) {
    lexpr->dump(dp);
    rexpr->dump(dp);
}

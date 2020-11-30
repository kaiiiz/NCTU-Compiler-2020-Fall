#include "AST/base/StatementBase.hpp"
#include "AST/base/ExpressionBase.hpp"
#include "AST/statement/print.hpp"
#include "AST/AstDumper.hpp"

PrintNode::PrintNode(const uint32_t line, const uint32_t col, std::shared_ptr<ExpressionBase> expr)
    : StatementBase{line, col}, expr(expr) {}

void PrintNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void PrintNode::dumpChildNodes(AstDumper &dp) {
    expr->dump(dp);
}

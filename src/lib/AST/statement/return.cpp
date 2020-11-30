#include "AST/base/ExpressionBase.hpp"
#include "AST/base/StatementBase.hpp"
#include "AST/statement/return.hpp"
#include "AST/AstDumper.hpp"

ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,
                       std::shared_ptr<ExpressionBase> expression)
    : StatementBase{line, col}, expression(expression) {}

void ReturnNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void ReturnNode::dumpChildNodes(AstDumper &dp) {
    expression->dump(dp);
}

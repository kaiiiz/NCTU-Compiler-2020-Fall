#include "AST/base/ExpressionBase.hpp"
#include "AST/base/StatementBase.hpp"
#include "AST/statement/while.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/AstDumper.hpp"

WhileNode::WhileNode(const uint32_t line, const uint32_t col,
                     std::shared_ptr<ExpressionBase> condition,
                     std::shared_ptr<CompoundStatementNode> body)
    : StatementBase{line, col}, condition(condition), body(body) {}

void WhileNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void WhileNode::dumpChildNodes(AstDumper &dp) {
    condition->dump(dp);
    body->dump(dp);
}

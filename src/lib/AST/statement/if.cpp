#include "AST/base/StatementBase.hpp"
#include "AST/base/ExpressionBase.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/statement/if.hpp"
#include "AST/AstDumper.hpp"

IfNode::IfNode(const uint32_t line, const uint32_t col,
           std::shared_ptr<ExpressionBase> condition,
           std::shared_ptr<CompoundStatementNode> body,
           std::shared_ptr<CompoundStatementNode> body_else)
    : StatementBase{line, col}, condition(condition), body(body), body_else(body_else) {}

void IfNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void IfNode::dumpChildNodes(AstDumper &dp) {
    condition->dump(dp);
    body->dump(dp);
    if (body_else != nullptr) {
        body_else->dump(dp);
    }
}

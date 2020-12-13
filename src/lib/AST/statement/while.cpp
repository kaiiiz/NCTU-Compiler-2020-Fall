#include "AST/base/ExpressionBase.hpp"
#include "AST/base/StatementBase.hpp"
#include "AST/statement/while.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "visitor/AstDumper.hpp"

WhileNode::WhileNode(const uint32_t line, const uint32_t col,
                     std::shared_ptr<ExpressionBase> condition,
                     std::shared_ptr<CompoundStatementNode> body)
    : StatementBase{line, col}, condition(condition), body(body) {}

void WhileNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    condition->accept(p_visitor);
    body->accept(p_visitor);
}

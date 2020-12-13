#include "AST/base/StatementBase.hpp"
#include "AST/base/ExpressionBase.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/statement/if.hpp"
#include "visitor/AstDumper.hpp"

IfNode::IfNode(const uint32_t line, const uint32_t col,
           std::shared_ptr<ExpressionBase> condition,
           std::shared_ptr<CompoundStatementNode> body,
           std::shared_ptr<CompoundStatementNode> body_else)
    : StatementBase{line, col}, condition(condition), body(body), body_else(body_else) {}

void IfNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    condition->accept(p_visitor);
    body->accept(p_visitor);
    if (body_else != nullptr) {
        body_else->accept(p_visitor);
    }
}

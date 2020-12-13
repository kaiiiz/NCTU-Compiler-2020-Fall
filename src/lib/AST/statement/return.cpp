#include "AST/base/ExpressionBase.hpp"
#include "AST/base/StatementBase.hpp"
#include "AST/statement/return.hpp"
#include "visitor/AstDumper.hpp"

ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,
                       std::shared_ptr<ExpressionBase> expression)
    : StatementBase{line, col}, expression(expression) {}

void ReturnNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expression->accept(p_visitor);
}

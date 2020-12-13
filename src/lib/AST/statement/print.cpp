#include "AST/base/StatementBase.hpp"
#include "AST/base/ExpressionBase.hpp"
#include "AST/statement/print.hpp"
#include "visitor/AstDumper.hpp"

PrintNode::PrintNode(const uint32_t line, const uint32_t col, std::shared_ptr<ExpressionBase> expr)
    : StatementBase{line, col}, expr(expr) {}

void PrintNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    expr->accept(p_visitor);
}

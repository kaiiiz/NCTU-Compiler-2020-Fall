#include "AST/base/StatementBase.hpp"
#include "AST/expression/FunctionInvocation.hpp"
#include "visitor/AstDumper.hpp"

FunctionInvocationNode::FunctionInvocationNode(const uint32_t line, const uint32_t col,
                           std::string name, std::vector<std::shared_ptr<ExpressionBase>> expressions)
    : ExpressionBase{line, col}, name(name), expressions(expressions) {}

std::string FunctionInvocationNode::getFuncName() { return name; }

std::vector<std::shared_ptr<ExpressionBase>> FunctionInvocationNode::getExprs() { return expressions; }

void FunctionInvocationNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &e : expressions) {
        e->accept(p_visitor);
    }
}

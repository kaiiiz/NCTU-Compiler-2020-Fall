#include "AST/base/StatementBase.hpp"
#include "AST/statement/FunctionCall.hpp"
#include "AST/expression/FunctionInvocation.hpp"
#include "visitor/AstDumper.hpp"

FunctionCallNode::FunctionCallNode(const uint32_t line, const uint32_t col,
                                   std::shared_ptr<FunctionInvocationNode> func_invoc)
    : StatementBase{line, col}, func_invoc(func_invoc) {}

void FunctionCallNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void FunctionCallNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    func_invoc->accept(p_visitor);
}

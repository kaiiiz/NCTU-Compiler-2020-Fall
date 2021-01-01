#include "AST/base/StatementBase.hpp"
#include "AST/statement/read.hpp"
#include "AST/expression/VariableReference.hpp"
#include "visitor/AstDumper.hpp"

ReadNode::ReadNode(const uint32_t line, const uint32_t col,
                   const std::shared_ptr<VariableReferenceNode> var_ref)
    : StatementBase{line, col}, var_ref(var_ref) {}

void ReadNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    var_ref->accept(p_visitor);
}

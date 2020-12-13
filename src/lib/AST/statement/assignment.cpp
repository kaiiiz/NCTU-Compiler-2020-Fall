#include "AST/base/StatementBase.hpp"
#include "AST/statement/assignment.hpp"
#include "AST/expression/VariableReference.hpp"
#include "visitor/AstDumper.hpp"

AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,
                               std::shared_ptr<VariableReferenceNode> var_ref,
                               std::shared_ptr<ExpressionBase> expression)
    : StatementBase{line, col}, var_ref(var_ref), expression(expression) {}

std::string AssignmentNode::getVarStr() { return var_ref->getNameStr(); }

void AssignmentNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    var_ref->accept(p_visitor);
    expression->accept(p_visitor);
}

#include "AST/base/StatementBase.hpp"
#include "AST/statement/assignment.hpp"
#include "AST/expression/VariableReference.hpp"
#include "AST/AstDumper.hpp"

AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,
                               std::shared_ptr<VariableReferenceNode> var_ref,
                               std::shared_ptr<ExpressionBase> expression)
    : StatementBase{line, col}, var_ref(var_ref), expression(expression) {}


void AssignmentNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void AssignmentNode::dumpChildNodes(AstDumper &dp) {
    var_ref->dump(dp);
    expression->dump(dp);
}

#include "AST/base/StatementBase.hpp"
#include "AST/expression/FunctionInvocation.hpp"

FunctionInvocationNode::FunctionInvocationNode(const uint32_t line, const uint32_t col)
    : ExpressionBase{line, col} {}

// void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

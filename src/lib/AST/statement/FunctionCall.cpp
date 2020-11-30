#include "AST/base/StatementBase.hpp"
#include "AST/statement/FunctionCall.hpp"

FunctionCallNode::FunctionCallNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

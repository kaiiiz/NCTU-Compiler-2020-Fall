#include "AST/base/StatementBase.hpp"
#include "AST/statement/if.hpp"

// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

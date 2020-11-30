#include "AST/base/StatementBase.hpp"
#include "AST/statement/for.hpp"

// TODO
ForNode::ForNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

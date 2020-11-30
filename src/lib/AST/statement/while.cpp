#include "AST/base/StatementBase.hpp"
#include "AST/statement/while.hpp"

// TODO
WhileNode::WhileNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

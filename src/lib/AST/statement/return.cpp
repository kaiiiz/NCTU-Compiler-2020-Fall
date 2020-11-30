#include "AST/base/StatementBase.hpp"
#include "AST/statement/return.hpp"

// TODO
ReturnNode::ReturnNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

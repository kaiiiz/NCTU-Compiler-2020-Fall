#include "AST/base/StatementBase.hpp"
#include "AST/statement/read.hpp"

// TODO
ReadNode::ReadNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

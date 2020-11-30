#include "AST/base/StatementBase.hpp"
#include "AST/statement/print.hpp"

// TODO
PrintNode::PrintNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

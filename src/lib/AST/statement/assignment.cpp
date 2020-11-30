#include "AST/base/StatementBase.hpp"
#include "AST/statement/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col)
    : StatementBase{line, col} {}

// void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }

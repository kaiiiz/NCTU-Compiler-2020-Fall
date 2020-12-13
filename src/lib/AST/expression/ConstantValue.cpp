#include "AST/base/ExpressionBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "visitor/AstDumper.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, TypeKind type)
    : ExpressionBase{line, col}, type(type) {}

TypeKind ConstantValueNode::getType() { return type; }

void ConstantValueNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ConstantValueNode::visitChildNodes(AstNodeVisitor &p_visitor) {
}

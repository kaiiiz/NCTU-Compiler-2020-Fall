#include "AST/base/ExpressionBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "visitor/AstDumper.hpp"
#include "type/struct.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, std::shared_ptr<TypeStruct> type)
    : ExpressionBase{line, col, type} {}

void ConstantValueNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ConstantValueNode::visitChildNodes(AstNodeVisitor &p_visitor) {
}

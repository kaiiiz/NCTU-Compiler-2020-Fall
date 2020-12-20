#include "AST/base/ExpressionBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "visitor/AstDumper.hpp"
#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, const int64_t ival)
    : ExpressionBase{line, col, type_mgr.getType(TypeKind::Integer)}, value_str(std::to_string(ival)) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, const bool bval)
    : ExpressionBase{line, col, type_mgr.getType(TypeKind::Boolean)}, value_str(std::to_string(bval)) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, const double rval)
    : ExpressionBase{line, col, type_mgr.getType(TypeKind::Real)}, value_str(std::to_string(rval)) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, const std::string sval)
    : ExpressionBase{line, col, type_mgr.getType(TypeKind::String)}, value_str(sval) {}

void ConstantValueNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ConstantValueNode::visitChildNodes(AstNodeVisitor &p_visitor) {
}

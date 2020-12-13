#include "AST/expression/ConstantValue/ConstBoolValue.hpp"

#include <string>

#include "AST/expression/ConstantValue.hpp"
#include "type/base.hpp"

ConstBoolValueNode::ConstBoolValueNode(const uint32_t line, const uint32_t col, bool bval)
    : ConstantValueNode{line, col, TypeKind::boolean}, bval(bval) {}

std::string ConstBoolValueNode::getValueString() {
    return (bval) ? "true" : "false";
}

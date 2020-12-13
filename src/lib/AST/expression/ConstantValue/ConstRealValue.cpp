#include "AST/expression/ConstantValue/ConstRealValue.hpp"

#include <string>

#include "AST/expression/ConstantValue.hpp"
#include "type/base.hpp"

ConstRealValueNode::ConstRealValueNode(const uint32_t line, const uint32_t col, double rval)
    : ConstantValueNode{line, col, TypeKind::real}, rval(rval) {}

std::string ConstRealValueNode::getValueString() {
    return std::to_string(rval);
}

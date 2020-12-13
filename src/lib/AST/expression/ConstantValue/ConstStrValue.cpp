#include "AST/expression/ConstantValue/ConstStrValue.hpp"

#include <string>

#include "AST/expression/ConstantValue.hpp"
#include "type/base.hpp"

ConstStrValueNode::ConstStrValueNode(const uint32_t line, const uint32_t col, std::string sval)
    : ConstantValueNode{line, col, TypeKind::string}, sval(sval) {}

std::string ConstStrValueNode::getValueString() {
    return sval;
}

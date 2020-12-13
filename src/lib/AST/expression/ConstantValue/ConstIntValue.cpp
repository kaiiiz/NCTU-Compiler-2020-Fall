#include "AST/expression/ConstantValue/ConstIntValue.hpp"

#include <string>

#include "AST/expression/ConstantValue.hpp"
#include "type/base.hpp"

ConstIntValueNode::ConstIntValueNode(const uint32_t line, const uint32_t col, int64_t ival)
    : ConstantValueNode{line, col, TypeKind::integer}, ival(ival) {}

std::string ConstIntValueNode::getValueString() {
    return std::to_string(ival);
}

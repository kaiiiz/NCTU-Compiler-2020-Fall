#include "AST/expression/ConstantValue/ConstIntValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

ConstIntValueNode::ConstIntValueNode(const uint32_t line, const uint32_t col, int64_t ival)
    : ConstantValueNode{line, col, type_mgr.getType(TypeKind::integer)}, ival(ival) {}

std::string ConstIntValueNode::getValueString() {
    return std::to_string(ival);
}

int64_t ConstIntValueNode::getValue() {
    return ival;
}

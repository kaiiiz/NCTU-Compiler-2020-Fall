#include "AST/expression/ConstantValue/ConstBoolValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

ConstBoolValueNode::ConstBoolValueNode(const uint32_t line, const uint32_t col, bool bval)
    : ConstantValueNode{line, col, type_mgr.getType(TypeKind::boolean)}, bval(bval) {}

std::string ConstBoolValueNode::getValueString() {
    return (bval) ? "true" : "false";
}

bool ConstBoolValueNode::getValue() { return bval; }

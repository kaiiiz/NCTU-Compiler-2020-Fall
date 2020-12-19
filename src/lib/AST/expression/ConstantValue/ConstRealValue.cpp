#include "AST/expression/ConstantValue/ConstRealValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

ConstRealValueNode::ConstRealValueNode(const uint32_t line, const uint32_t col, double rval)
    : ConstantValueNode{line, col, type_mgr.getType(TypeKind::Real)}, rval(rval) {}

std::string ConstRealValueNode::getValueString() {
    return std::to_string(rval);
}

double ConstRealValueNode::getValue() {
    return rval;
}

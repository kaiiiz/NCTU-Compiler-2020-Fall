#include "AST/expression/ConstantValue/ConstStrValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/struct.hpp"
#include "type/manager.hpp"

extern TypeManager type_mgr;

ConstStrValueNode::ConstStrValueNode(const uint32_t line, const uint32_t col, std::string sval)
    : ConstantValueNode{line, col, type_mgr.getType(TypeKind::string)}, sval(sval) {}

std::string ConstStrValueNode::getValueString() {
    return sval;
}

std::string ConstStrValueNode::getValue() {
    return sval;
}

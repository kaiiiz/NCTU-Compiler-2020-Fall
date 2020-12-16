#include "AST/expression/ConstantValue/ConstStrValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/struct.hpp"

ConstStrValueNode::ConstStrValueNode(const uint32_t line, const uint32_t col, std::string sval)
    : ConstantValueNode{line, col, std::make_shared<TypeStruct>(TypeKind::string)}, sval(sval) {}

std::string ConstStrValueNode::getValueString() {
    return sval;
}

std::string ConstStrValueNode::getValue() {
    return sval;
}

#include "AST/expression/ConstantValue/ConstStrValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/scalar.hpp"

ConstStrValueNode::ConstStrValueNode(const uint32_t line, const uint32_t col, std::string sval)
    : ConstantValueNode{line, col, std::make_shared<ScalarType>(TypeKind::string)}, sval(sval) {}

std::string ConstStrValueNode::getValueString() {
    return sval;
}

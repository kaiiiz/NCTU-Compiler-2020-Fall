#include "AST/expression/ConstantValue/ConstRealValue.hpp"

#include <string>
#include <memory>

#include "AST/expression/ConstantValue.hpp"
#include "type/scalar.hpp"

ConstRealValueNode::ConstRealValueNode(const uint32_t line, const uint32_t col, double rval)
    : ConstantValueNode{line, col, std::make_shared<ScalarType>(TypeKind::real)}, rval(rval) {}

std::string ConstRealValueNode::getValueString() {
    return std::to_string(rval);
}

#include "AST/ConstantValue.hpp"
#include "AST/AstDumper.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, int64_t ival)
    : ExpressionNode{line, col}, type(type), ival(ival) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, bool bval)
    : ExpressionNode{line, col}, type(type), bval(bval) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, double rval)
    : ExpressionNode{line, col}, type(type), rval(rval) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, std::string sval)
    : ExpressionNode{line, col}, type(type), sval(sval) {}

scalar_type_t ConstantValueNode::getType() { return type; }

std::string ConstantValueNode::getValueString() {
    switch (type) {
        case scalar_type_t::boolean:
            return (bval) ? "true" : "false";
        case scalar_type_t::integer:
            return std::to_string(ival);
        case scalar_type_t::string:
            return sval;
        case scalar_type_t::real:
            return std::to_string(rval);
    }
    return "";
}

void ConstantValueNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void ConstantValueNode::dumpChildNodes(AstDumper &dp) {
}

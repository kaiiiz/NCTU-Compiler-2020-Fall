#include "AST/base/ExpressionBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "AST/AstDumper.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, int64_t ival)
    : ExpressionBase{line, col}, type(type), ival(ival) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, bool bval)
    : ExpressionBase{line, col}, type(type), bval(bval) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, double rval)
    : ExpressionBase{line, col}, type(type), rval(rval) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     scalar_type_t type, std::string sval)
    : ExpressionBase{line, col}, type(type), sval(sval) {}

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

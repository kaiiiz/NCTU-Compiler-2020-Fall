#include "AST/base/ExpressionBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "visitor/AstDumper.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     TypeKind type, int64_t ival)
    : ExpressionBase{line, col}, type(type), ival(ival) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     TypeKind type, bool bval)
    : ExpressionBase{line, col}, type(type), bval(bval) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     TypeKind type, double rval)
    : ExpressionBase{line, col}, type(type), rval(rval) {}

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     TypeKind type, std::string sval)
    : ExpressionBase{line, col}, type(type), sval(sval) {}

TypeKind ConstantValueNode::getType() { return type; }

std::string ConstantValueNode::getValueString() {
    switch (type) {
        case TypeKind::boolean:
            return (bval) ? "true" : "false";
        case TypeKind::integer:
            return std::to_string(ival);
        case TypeKind::string:
            return sval;
        case TypeKind::real:
            return std::to_string(rval);
        default:
            return "<not handled constant value type>";
    }
}

void ConstantValueNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ConstantValueNode::visitChildNodes(AstNodeVisitor &p_visitor) {
}

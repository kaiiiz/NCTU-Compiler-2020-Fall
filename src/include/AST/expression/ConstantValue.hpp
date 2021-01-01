#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"
#include "type/struct.hpp"

class VariableNode;

class ConstantValueNode : public ExpressionBase {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col, const int64_t ival);
    ConstantValueNode(const uint32_t line, const uint32_t col, const bool bval);
    ConstantValueNode(const uint32_t line, const uint32_t col, const double rval);
    ConstantValueNode(const uint32_t line, const uint32_t col, const std::string sval);
    ~ConstantValueNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    const std::string value_str;
};

#endif

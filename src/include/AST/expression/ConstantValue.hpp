#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"
#include "type/scalar.hpp"

class BaseType;
class VariableNode;

class ConstantValueNode : public ExpressionBase {
  public:
    friend class VariableNode;
    ConstantValueNode(const uint32_t line, const uint32_t col, scalar_type_t type, int64_t ival);
    ConstantValueNode(const uint32_t line, const uint32_t col, scalar_type_t type, bool bval);
    ConstantValueNode(const uint32_t line, const uint32_t col, scalar_type_t type, double dval);
    ConstantValueNode(const uint32_t line, const uint32_t col, scalar_type_t type, std::string sval);
    ~ConstantValueNode() = default;

    scalar_type_t getType();
    std::string getValueString();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    scalar_type_t type;
    int64_t ival;
    bool bval;
    double rval;
    std::string sval;
};

#endif

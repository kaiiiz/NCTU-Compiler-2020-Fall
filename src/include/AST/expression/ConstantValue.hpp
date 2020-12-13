#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"
#include "type/scalar.hpp"

class VariableNode;

class ConstantValueNode : public ExpressionBase {
  public:
    friend class VariableNode;
    ConstantValueNode(const uint32_t line, const uint32_t col, TypeKind type, int64_t ival);
    ConstantValueNode(const uint32_t line, const uint32_t col, TypeKind type, bool bval);
    ConstantValueNode(const uint32_t line, const uint32_t col, TypeKind type, double dval);
    ConstantValueNode(const uint32_t line, const uint32_t col, TypeKind type, std::string sval);
    ~ConstantValueNode() = default;

    TypeKind getType();
    std::string getValueString();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    TypeKind type;
    int64_t ival;
    bool bval;
    double rval;
    std::string sval;
};

#endif

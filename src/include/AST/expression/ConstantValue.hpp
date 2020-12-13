#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"
#include "type/scalar.hpp"

class VariableNode;

class ConstantValueNode : public ExpressionBase {
  public:
    friend class VariableNode;
    ConstantValueNode(const uint32_t line, const uint32_t col, TypeKind type);
    ~ConstantValueNode() = default;

    TypeKind getType();
    virtual std::string getValueString() = 0;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    TypeKind type;
};

#endif

#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"
#include "type/base.hpp"

class VariableNode;

class ConstantValueNode : public ExpressionBase {
  public:
    friend class VariableNode;
    ConstantValueNode(const uint32_t line, const uint32_t col, std::shared_ptr<TypeBase> type);
    ~ConstantValueNode() = default;

    std::shared_ptr<TypeBase> getType();
    virtual std::string getValueString() = 0;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::shared_ptr<TypeBase> type;
};

#endif

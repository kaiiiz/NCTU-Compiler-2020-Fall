#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include <memory>

#include "AST/base/ExpressionBase.hpp"
#include "type/struct.hpp"

class VariableNode;

class ConstantValueNode : public ExpressionBase {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col, std::shared_ptr<TypeStruct> type);
    ~ConstantValueNode() = default;

    std::shared_ptr<TypeStruct> getType();
    virtual std::string getValueString() = 0;
    template<typename T>
    std::shared_ptr<T> getConstValueNode();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::shared_ptr<TypeStruct> type;
};

#endif

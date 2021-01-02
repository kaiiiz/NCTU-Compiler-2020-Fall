#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include <vector>
#include <memory>

#include "AST/base/ExpressionBase.hpp"

enum class Side {
    LHS,
    RHS,
};

class VariableReferenceNode : public ExpressionBase {
  public:
    VariableReferenceNode(const uint32_t line, const uint32_t col, const std::string name, 
                          const std::vector<std::shared_ptr<ExpressionBase>> expressions);
    ~VariableReferenceNode() = default;

    const std::string name;
    const std::vector<std::shared_ptr<ExpressionBase>> expressions;

    void fillSide(Side side);
    Side getSide();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    Side side;
};

#endif

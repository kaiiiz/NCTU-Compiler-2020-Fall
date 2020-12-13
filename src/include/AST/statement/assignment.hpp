#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class VariableReferenceNode;
class ExpressionBase;

class AssignmentNode : public StatementBase {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col,
                   std::shared_ptr<VariableReferenceNode> var_ref,
                   std::shared_ptr<ExpressionBase> expression);
    ~AssignmentNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::shared_ptr<VariableReferenceNode> var_ref;
    std::shared_ptr<ExpressionBase> expression;
};

#endif

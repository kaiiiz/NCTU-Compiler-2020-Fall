#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class VariableReferenceNode;
class ExpressionBase;

class AssignmentNode : public StatementBase {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col,
                   const std::shared_ptr<VariableReferenceNode> var_ref,
                   const std::shared_ptr<ExpressionBase> expression);
    ~AssignmentNode() = default;

    const std::shared_ptr<VariableReferenceNode> var_ref;
    const std::shared_ptr<ExpressionBase> expression;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

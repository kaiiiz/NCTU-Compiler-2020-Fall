#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include <vector>
#include <memory>

#include "AST/base/ExpressionBase.hpp"

class FunctionInvocationNode : public ExpressionBase {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col, const std::string name,
                           const std::vector<std::shared_ptr<ExpressionBase>> expressions);
    ~FunctionInvocationNode() = default;

    const std::string name;
    const std::vector<std::shared_ptr<ExpressionBase>> expressions;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

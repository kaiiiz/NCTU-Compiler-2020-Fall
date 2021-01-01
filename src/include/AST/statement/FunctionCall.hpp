#ifndef __AST_FUNCTION_CALL_NODE_H
#define __AST_FUNCTION_CALL_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class FunctionInvocationNode;

class FunctionCallNode : public StatementBase {
  public:
    FunctionCallNode(const uint32_t line, const uint32_t col,
                     std::shared_ptr<FunctionInvocationNode> func_invoc);
    ~FunctionCallNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::shared_ptr<FunctionInvocationNode> func_invoc;
};

#endif

#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include <vector>
#include <memory>

#include "AST/base/ExpressionBase.hpp"

class FunctionInvocationNode : public ExpressionBase {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col,
                           std::string name, std::vector<std::shared_ptr<ExpressionBase>> expressions);
    ~FunctionInvocationNode() = default;

    std::string getFuncName();
    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::string name;
    std::vector<std::shared_ptr<ExpressionBase>> expressions;
};

#endif

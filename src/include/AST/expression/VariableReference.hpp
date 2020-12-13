#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include <vector>
#include <memory>

#include "AST/base/ExpressionBase.hpp"

class VariableReferenceNode : public ExpressionBase {
  public:
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          std::string name, std::vector<std::shared_ptr<ExpressionBase>> expressions);
    ~VariableReferenceNode() = default;

    std::string getName();
    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::string name;
    std::vector<std::shared_ptr<ExpressionBase>> expressions;
};

#endif

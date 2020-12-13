#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"

class VariableReferenceNode;

class ReadNode : public StatementBase {
  public:
    ReadNode(const uint32_t line, const uint32_t col,
             std::shared_ptr<VariableReferenceNode> var_ref);
    ~ReadNode() = default;

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::shared_ptr<VariableReferenceNode> var_ref;
};

#endif

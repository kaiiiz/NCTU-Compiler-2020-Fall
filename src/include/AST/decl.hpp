#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include <vector>
#include <string>
#include <memory>

#include "AST/ast.hpp"

class TypeStruct;
class VariableNode;

class DeclNode : public AstNode {
  public:
    DeclNode(const uint32_t line, const uint32_t col,
             std::vector<std::shared_ptr<VariableNode>> var_list,
             const std::shared_ptr<TypeStruct> type);

    ~DeclNode() = default;

    const std::shared_ptr<TypeStruct> type;
    int32_t getVarNum();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::vector<std::shared_ptr<VariableNode>> var_list;
};

#endif

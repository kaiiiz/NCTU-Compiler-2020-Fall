#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include <vector>
#include <string>
#include <memory>

#include "AST/ast.hpp"

class TypeBase;
class ArrayType;
class VariableNode;

class DeclNode : public AstNode {
  public:
    DeclNode(const uint32_t line, const uint32_t col,
             std::vector<std::shared_ptr<VariableNode>> var_list,
             std::shared_ptr<TypeBase> type);

    ~DeclNode() = default;

    std::string getType();
    int32_t getVarNum();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::vector<std::shared_ptr<VariableNode>> var_list;
    std::shared_ptr<TypeBase> type;
};

#endif

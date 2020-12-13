#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include <string>
#include <memory>

#include "AST/ast.hpp"

class TypeBase;
class ConstantValueNode;

class IdNode { // keep line and col information
  public:
    friend class VariableNode;
    IdNode(const uint32_t line, const uint32_t col, std::string name);
    ~IdNode() = default;

  protected:
    uint32_t line;
    uint32_t col;
    std::string name;
};

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col,
                 std::string name, std::shared_ptr<TypeBase> type);
    VariableNode(std::shared_ptr<IdNode> idnode, std::shared_ptr<TypeBase> type);
    VariableNode(std::shared_ptr<IdNode> idnode, std::shared_ptr<ConstantValueNode> literal_const);
    ~VariableNode() = default;

    std::string getName();
    std::string getTypeStr();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  protected:
    std::string name;
    std::shared_ptr<TypeBase> type;
    std::shared_ptr<ConstantValueNode> literal_const;
};

#endif

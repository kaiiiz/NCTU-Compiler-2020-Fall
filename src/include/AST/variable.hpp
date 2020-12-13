#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include <string>
#include <memory>

#include "AST/ast.hpp"

class TypeBase;
class ConstantValueNode;

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col, std::string name);
    VariableNode(const uint32_t line, const uint32_t col, std::string name, std::shared_ptr<TypeBase> type);
    ~VariableNode() = default;

    void fillAttribute(std::shared_ptr<TypeBase> type);
    void fillAttribute(std::shared_ptr<ConstantValueNode> literal_const);
    std::string getNameStr();
    std::string getTypeStr();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  protected:
    std::string name;
    std::shared_ptr<TypeBase> type;
    std::shared_ptr<ConstantValueNode> literal_const;
};

#endif

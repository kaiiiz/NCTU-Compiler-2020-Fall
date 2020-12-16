#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include <memory>
#include <string>

#include "AST/ast.hpp"

class TypeStruct;
class ConstantValueNode;

enum class VariableKind {
    parameter,
    variable,
    loop_var,
    constant,
};

class VariableNode : public AstNode {
   public:
    VariableNode(const uint32_t line, const uint32_t col, std::string name);
    VariableNode(const uint32_t line, const uint32_t col, std::string name, std::shared_ptr<TypeStruct> type);
    ~VariableNode() = default;

    void fillAttribute(std::shared_ptr<TypeStruct> type);
    void fillAttribute(std::shared_ptr<ConstantValueNode> literal_const);
    void fillAttribute(VariableKind kind);
    std::string getNameStr();
    std::string getTypeStr();

    template <class T>
    std::shared_ptr<T> getLiteralConst() {
        return std::dynamic_pointer_cast<T>(literal_const);
    }

    std::shared_ptr<TypeStruct> getType();
    VariableKind getVarKind();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

   protected:
    std::string name;
    std::shared_ptr<TypeStruct> type;
    std::shared_ptr<ConstantValueNode> literal_const;
    VariableKind kind;
};

#endif

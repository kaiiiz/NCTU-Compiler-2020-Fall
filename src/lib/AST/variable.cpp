#include <memory>

#include "AST/variable.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "visitor/AstDumper.hpp"
#include "type/base.hpp"
#include "type/scalar.hpp"

IdNode::IdNode(const uint32_t line, const uint32_t col, std::string name)
    : line(line), col(col), name(name) {}

VariableNode::VariableNode(const uint32_t line, const uint32_t col,
                           std::string name, std::shared_ptr<BaseType> type)
    : AstNode{line, col}, name(name), type(type) {}

VariableNode::VariableNode(std::shared_ptr<IdNode> idnode, std::shared_ptr<BaseType> type) 
    : AstNode{idnode->line, idnode->col}, name(idnode->name), type(type) {}

VariableNode::VariableNode(std::shared_ptr<IdNode> idnode, std::shared_ptr<ConstantValueNode> literal_const)
    : AstNode{idnode->line, idnode->col}, name(idnode->name),
      type(std::make_shared<ScalarType>(literal_const->getType())), literal_const(literal_const) {}

std::string VariableNode::getName() { return name; }

std::string VariableNode::getType() { return type->getTypeName(); }

void VariableNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (literal_const != nullptr) {
        literal_const->accept(p_visitor);
    }
}

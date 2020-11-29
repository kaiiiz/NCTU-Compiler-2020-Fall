#include <memory>

#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/AstDumper.hpp"
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

void VariableNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void VariableNode::dumpChildNodes(AstDumper &dp) {
    if (literal_const != nullptr) {
        dp.visit(*literal_const);
    }
}

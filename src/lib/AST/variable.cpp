#include "AST/variable.hpp"
#include "AST/AstDumper.hpp"

IdNode::IdNode(const uint32_t line, const uint32_t col, std::string name)
    : line(line), col(col), name(name) {}

VariableNode::VariableNode(const uint32_t line, const uint32_t col,
                           std::string name, std::shared_ptr<BaseType> type)
    : AstNode{line, col}, name(name), type(type) {}

VariableNode::VariableNode(std::shared_ptr<IdNode> idnode, std::shared_ptr<BaseType> type) 
    : AstNode{idnode->line, idnode->col}, name(idnode->name), type(type) {}

std::string VariableNode::getName() { return name; }

std::string VariableNode::getType() { return type->getTypeName(); }

void VariableNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void VariableNode::dumpChildNodes(AstDumper &dp) {}

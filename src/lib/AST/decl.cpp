#include <vector>
#include <string>

#include "visitor/AstDumper.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "type/base.hpp"

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   std::vector<std::shared_ptr<VariableNode>> var_list,
                   std::shared_ptr<TypeBase> type)
    : AstNode{line, col}, var_list(var_list), type(type) {}

std::shared_ptr<TypeBase> DeclNode::getType() { return type; }

std::string DeclNode::getTypeStr() { return type->getTypeStr(); }

int32_t DeclNode::getVarNum() { return var_list.size(); }

std::vector<std::shared_ptr<VariableNode>> DeclNode::getVarList() { return var_list; }

void DeclNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &var : var_list) {
        var->accept(p_visitor);
    }
}

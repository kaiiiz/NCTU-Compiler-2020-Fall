#include <vector>
#include <string>

#include "visitor/AstDumper.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "type/struct.hpp"

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   const std::vector<std::shared_ptr<VariableNode>> var_list,
                   std::shared_ptr<TypeStruct> type)
    : AstNode{line, col}, type(type), var_list(var_list) {}

int32_t DeclNode::getVarNum() { return var_list.size(); }

void DeclNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &var : var_list) {
        var->accept(p_visitor);
    }
}

#include <vector>
#include <string>

#include "AST/AstDumper.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "type/base.hpp"

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   std::vector<std::shared_ptr<VariableNode>> var_list,
                   std::shared_ptr<BaseType> type)
    : AstNode{line, col}, var_list(var_list), type(type) {}

std::string DeclNode::getType() { return type->getTypeName(); }

int32_t DeclNode::getVarNum() { return var_list.size(); }

void DeclNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void DeclNode::dumpChildNodes(AstDumper &dp) {
    for (auto &var : var_list) {
        var->dump(dp);
    }
}

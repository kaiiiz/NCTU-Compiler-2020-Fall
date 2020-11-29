#include <vector>
#include <string>

#include "AST/AstDumper.hpp"
#include "AST/decl.hpp"
#include "type/scalar.hpp"

DeclNode::DeclNode(const uint32_t line, const uint32_t col,
                   std::vector<std::shared_ptr<VariableNode>> var_list)
    : AstNode{line, col}, var_list(var_list) {}

void DeclNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void DeclNode::dumpChildNodes(AstDumper &dp) {
    for (auto &var : var_list) {
        dp.visit(*var);
    }
}

// TODO
//DeclNode::DeclNode(const uint32_t line, const uint32_t col)
//    : AstNode{line, col} {}

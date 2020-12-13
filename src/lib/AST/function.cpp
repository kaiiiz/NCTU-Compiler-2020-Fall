#include <string>
#include <memory>

#include "AST/function.hpp"
#include "AST/decl.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/AstDumper.hpp"

FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,
                           std::string name, std::vector<std::shared_ptr<DeclNode>> parameters,
                           std::shared_ptr<BaseType> return_type)
    : AstNode{line, col}, name(name), parameters(parameters), return_type(return_type) {}


FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,
                           std::string name, std::vector<std::shared_ptr<DeclNode>> parameters,
                           std::shared_ptr<BaseType> return_type,
                           std::shared_ptr<CompoundStatementNode> compound_stmt)
    : AstNode{line, col}, name(name), parameters(parameters),
      return_type(return_type), compound_stmt(compound_stmt) {}

std::string FunctionNode::getName() { return name; }

std::string FunctionNode::getFuncProtoType() {
    std::string s = return_type->getTypeName();
    s += " (";
    for (auto &p : parameters) {
        for (int i = 0; i < p->getVarNum(); i++) {
            s += p->getType();
            if (i != p->getVarNum() - 1) s += ", ";
        }
        if (&p != &parameters.back()) s += ", ";
    }
    s += ")";
    return s;
}

void FunctionNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void FunctionNode::dumpChildNodes(AstDumper &dp) {
    for (auto& p : parameters) {
        p->dump(dp);
    }

    if (compound_stmt != nullptr) {
        compound_stmt->dump(dp);
    }
}

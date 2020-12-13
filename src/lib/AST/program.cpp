#include <memory>

#include "visitor/AstDumper.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include "type/base.hpp"

ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                        const std::string p_name, std::shared_ptr<TypeBase> ret_type,
                        const std::vector<std::shared_ptr<DeclNode>> decl_list,
                        const std::vector<std::shared_ptr<FunctionNode>> func_list,
                        const std::shared_ptr<CompoundStatementNode> compound_stmt)
    : AstNode{line, col}, name(p_name), ret_type(ret_type), decl_list(decl_list), func_list(func_list), compound_stmt(compound_stmt) {}

void ProgramNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &decl : decl_list) {
        decl->accept(p_visitor);
    }

    for (auto &func : func_list) {
        func->accept(p_visitor);
    }

    compound_stmt->accept(p_visitor);
}

std::string ProgramNode::getProgramName() { return name; }

std::shared_ptr<TypeBase> ProgramNode::getReturnType() { return ret_type; }

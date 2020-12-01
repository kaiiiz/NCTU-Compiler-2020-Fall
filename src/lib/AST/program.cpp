#include "AST/AstDumper.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                        const std::string p_name, const std::string ret_type,
                        const std::vector<std::shared_ptr<DeclNode>> decl_list,
                        const std::vector<std::shared_ptr<FunctionNode>> func_list,
                        const std::shared_ptr<CompoundStatementNode> compound_stmt)
    : AstNode{line, col}, name(p_name), ret_type(ret_type), decl_list(decl_list), func_list(func_list), compound_stmt(compound_stmt) {}

void ProgramNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void ProgramNode::dumpChildNodes(AstDumper &dp) {
    for (auto &decl : decl_list) {
        decl->dump(dp);
    }

    for (auto &func : func_list) {
        func->dump(dp);
    }

    compound_stmt->dump(dp);
}

std::string ProgramNode::getProgramName() { return name; }

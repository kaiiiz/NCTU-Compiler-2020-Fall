#include "AST/AstDumper.hpp"
#include "AST/program.hpp"
#include "visitor/AstNodeVisitor.hpp"

ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                        const std::string p_name, const std::string ret_type,
                        const std::vector<std::shared_ptr<AstNode>> decl_list,
                        const std::vector<std::shared_ptr<AstNode>> func_list,
                        const std::shared_ptr<AstNode> compound_stmt)
    : AstNode{line, col}, name(p_name), ret_type(ret_type), decl_list(decl_list), func_list(func_list), compound_stmt(compound_stmt) {}

void ProgramNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

const char * ProgramNode::getProgramName() {
    return name.c_str();
}

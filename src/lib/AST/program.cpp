#include "AST/program.hpp"
#include <iostream>

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                        const std::string p_name, const std::string ret_type,
                        const std::vector<std::shared_ptr<AstNode>> decl_list,
                        const std::vector<std::shared_ptr<AstNode>> func_list,
                        const std::shared_ptr<AstNode> compound_stmt)
    : AstNode{line, col}, name(p_name), ret_type(ret_type), decl_list(decl_list), func_list(func_list), compound_stmt(compound_stmt) {}

// visitor pattern version: const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::print() {
    // TODO
    // outputIndentationSpace();

    std::printf("program <line: %u, col: %u> %s %s\n",
                location.line, location.col,
                name.c_str(), "void");

    // TODO
    // incrementIndentation();
    // visitChildNodes();
    // decrementIndentation();
}


// void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
//     /* TODO
//      *
//      * for (auto &decl : var_decls) {
//      *     decl->accept(p_visitor);
//      * }
//      *
//      * // functions
//      *
//      * body->accept(p_visitor);
//      */
// }

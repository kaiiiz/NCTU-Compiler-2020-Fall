#include "AST/statement/CompoundStatement.hpp"
#include "AST/AstDumper.hpp"

CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col,
                                             std::vector<std::shared_ptr<DeclNode>> declarations,
                                             std::vector<std::shared_ptr<StatementNode>> statements)
    : StatementNode{line, col}, declarations(declarations), statements(statements) {}

void CompoundStatementNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void CompoundStatementNode::dumpChildNodes(AstDumper &dp) {
}

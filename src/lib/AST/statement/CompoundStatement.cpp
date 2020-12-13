#include "AST/base/StatementBase.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/decl.hpp"
#include "AST/AstDumper.hpp"

CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col,
                                             std::vector<std::shared_ptr<DeclNode>> declarations,
                                             std::vector<std::shared_ptr<StatementBase>> statements)
    : StatementBase{line, col}, declarations(declarations), statements(statements) {}

void CompoundStatementNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void CompoundStatementNode::dumpChildNodes(AstDumper &dp) {
    for (auto &d : declarations) {
        d->dump(dp);
    }

    for (auto &s : statements) {
        s->dump(dp);
    }
}

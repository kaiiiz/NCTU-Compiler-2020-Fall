#include "AST/base/StatementBase.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/decl.hpp"
#include "visitor/AstDumper.hpp"

CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col,
                                             std::vector<std::shared_ptr<DeclNode>> declarations,
                                             std::vector<std::shared_ptr<StatementBase>> statements)
    : StatementBase{line, col}, declarations(declarations), statements(statements) {}

void CompoundStatementNode::fillAttribute(CompoundKind k) { kind = k; }

CompoundKind CompoundStatementNode::getKind() { return kind; }

void CompoundStatementNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &d : declarations) {
        d->accept(p_visitor);
    }

    for (auto &s : statements) {
        s->accept(p_visitor);
    }
}

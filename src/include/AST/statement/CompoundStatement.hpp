#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include <memory>
#include <vector>

#include "AST/base/StatementBase.hpp"

class DeclNode;

enum CompoundKind {
    function,
    normal,
};

class CompoundStatementNode : public StatementBase {
   public:
    CompoundStatementNode(const uint32_t line, const uint32_t col,
                          std::vector<std::shared_ptr<DeclNode>> declarations,
                          std::vector<std::shared_ptr<StatementBase>> statements);
    ~CompoundStatementNode() = default;

    void fillAttribute(CompoundKind kind);
    CompoundKind getKind();
    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

   private:
    std::vector<std::shared_ptr<DeclNode>> declarations;
    std::vector<std::shared_ptr<StatementBase>> statements;
    CompoundKind kind;
};

#endif

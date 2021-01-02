#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include <memory>

#include "AST/base/StatementBase.hpp"
#include "AST/base/AstNodeWithSymTab.hpp"

class DeclNode;
class AssignmentNode;
class ConstantValueNode;
class CompoundStatementNode;

class ForNode : public StatementBase, public AstNodeWithSymTab {
  public:
    ForNode(const uint32_t line, const uint32_t col,
            const std::shared_ptr<DeclNode> declaration,
            const std::shared_ptr<AssignmentNode> assignment,
            const std::shared_ptr<ConstantValueNode> condition,
            const std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~ForNode() = default;

    const std::shared_ptr<DeclNode> declaration;
    const std::shared_ptr<AssignmentNode> assignment;
    const std::shared_ptr<ConstantValueNode> condition;
    const std::shared_ptr<CompoundStatementNode> compound_stmt;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
};

#endif

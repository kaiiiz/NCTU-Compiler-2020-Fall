#include "AST/base/StatementBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "AST/statement/for.hpp"
#include "AST/statement/assignment.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/AstDumper.hpp"
#include "AST/decl.hpp"

ForNode::ForNode(const uint32_t line, const uint32_t col,
                 std::shared_ptr<DeclNode> declaration,
                 std::shared_ptr<AssignmentNode> assignment,
                 std::shared_ptr<ConstantValueNode> condition,
                 std::shared_ptr<CompoundStatementNode> compound_stmt)
    : StatementBase{line, col}, declaration(declaration), assignment(assignment)
                , condition(condition), compound_stmt(compound_stmt) {}

void ForNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void ForNode::dumpChildNodes(AstDumper &dp) {
    declaration->dump(dp);
    assignment->dump(dp);
    condition->dump(dp);
    compound_stmt->dump(dp);
}

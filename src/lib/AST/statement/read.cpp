#include "AST/base/StatementBase.hpp"
#include "AST/statement/read.hpp"
#include "AST/expression/VariableReference.hpp"
#include "AST/AstDumper.hpp"

ReadNode::ReadNode(const uint32_t line, const uint32_t col,
                   std::shared_ptr<VariableReferenceNode> var_ref)
    : StatementBase{line, col}, var_ref(var_ref) {}

void ReadNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void ReadNode::dumpChildNodes(AstDumper &dp) {
    var_ref->dump(dp);
}

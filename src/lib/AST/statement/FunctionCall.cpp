#include "AST/base/StatementBase.hpp"
#include "AST/statement/FunctionCall.hpp"
#include "AST/expression/FunctionInvocation.hpp"
#include "AST/AstDumper.hpp"

FunctionCallNode::FunctionCallNode(const uint32_t line, const uint32_t col,
                                   std::shared_ptr<FunctionInvocationNode> func_invoc)
    : StatementBase{line, col}, func_invoc(func_invoc) {}

void FunctionCallNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void FunctionCallNode::dumpChildNodes(AstDumper &dp) {
    func_invoc->dump(dp);
}

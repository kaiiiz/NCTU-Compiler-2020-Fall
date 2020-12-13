#include "AST/base/StatementBase.hpp"
#include "AST/expression/FunctionInvocation.hpp"
#include "AST/AstDumper.hpp"

FunctionInvocationNode::FunctionInvocationNode(const uint32_t line, const uint32_t col,
                           std::string name, std::vector<std::shared_ptr<ExpressionBase>> expressions)
    : ExpressionBase{line, col}, name(name), expressions(expressions) {}

std::string FunctionInvocationNode::getFuncName() { return name; }

void FunctionInvocationNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void FunctionInvocationNode::dumpChildNodes(AstDumper &dp) {
    for (auto &e : expressions) {
        e->dump(dp);
    }
}

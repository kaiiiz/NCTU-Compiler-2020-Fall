#include "AST/expression/VariableReference.hpp"
#include "AST/AstDumper.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line, const uint32_t col,
                                             std::string name,
                                             std::vector<std::shared_ptr<ExpressionBase>> expressions)
    : ExpressionBase{line, col}, name(name), expressions(expressions) {}

std::string VariableReferenceNode::getName() { return name; }

void VariableReferenceNode::dump(AstDumper &dp) {
    dp.visit(*this);
}

void VariableReferenceNode::dumpChildNodes(AstDumper &dp) {
    for (auto &e : expressions) {
        e->dump(dp);
    }
}

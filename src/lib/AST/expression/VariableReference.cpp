#include "AST/expression/VariableReference.hpp"
#include "visitor/AstDumper.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line, const uint32_t col,
                                             std::string name,
                                             std::vector<std::shared_ptr<ExpressionBase>> expressions)
    : ExpressionBase{line, col}, name(name), expressions(expressions) {}

void VariableReferenceNode::fillSide(Side s) { side = s; }

Side VariableReferenceNode::getSide() { return side; }

void VariableReferenceNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &e : expressions) {
        e->accept(p_visitor);
    }
}

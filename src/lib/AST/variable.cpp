#include <memory>

#include "AST/variable.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "visitor/AstDumper.hpp"
#include "type/base.hpp"
#include "type/scalar.hpp"

VariableNode::VariableNode(const uint32_t line, const uint32_t col,
                           std::string name)
    : AstNode{line, col}, name(name) {}

VariableNode::VariableNode(const uint32_t line, const uint32_t col,
                           std::string name, std::shared_ptr<TypeBase> type)
    : AstNode{line, col}, name(name), type(type) {}

void VariableNode::fillAttribute(std::shared_ptr<TypeBase> type_attr) {
    if (type != nullptr) {
        return;
    }
    type = type_attr;
}

void VariableNode::fillAttribute(std::shared_ptr<ConstantValueNode> const_attr) {
    if (type != nullptr || literal_const != nullptr) {
        return;
    }
    type = const_attr->getType();
    literal_const = const_attr;
}

void VariableNode::fillAttribute(VariableKind var_kind) {
    kind = var_kind;
}

std::shared_ptr<ConstantValueNode> VariableNode::getLiteralConst() { return literal_const; }

std::shared_ptr<TypeBase> VariableNode::getType() { return type; }

std::string VariableNode::getNameStr() { return name; }

std::string VariableNode::getTypeStr() {
    if (type == nullptr) {
        return "";
    }
    return type->getTypeStr();
}

VariableKind VariableNode::getVarKind() { return kind; }

void VariableNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (literal_const != nullptr) {
        literal_const->accept(p_visitor);
    }
}

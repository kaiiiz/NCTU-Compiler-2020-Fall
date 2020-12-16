#include "AST/base/ExpressionBase.hpp"
#include "type/struct.hpp"

ExpressionBase::ExpressionBase(const uint32_t line, const uint32_t col) : AstNode{line, col} {}

ExpressionBase::ExpressionBase(const uint32_t line, const uint32_t col, std::shared_ptr<TypeStruct> type) : AstNode{line, col}, type(type) {}

ExpressionBase::~ExpressionBase() {}

std::shared_ptr<TypeStruct> ExpressionBase::getType() { return type; }

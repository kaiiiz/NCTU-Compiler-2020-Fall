#include "AST/base/ExpressionBase.hpp"

ExpressionBase::ExpressionBase(const uint32_t line, const uint32_t col) : AstNode{line, col} {}

ExpressionBase::~ExpressionBase() {}

#include "AST/base/StatementBase.hpp"

StatementBase::StatementBase(const uint32_t line, const uint32_t col) : AstNode{line, col} {}

StatementBase::~StatementBase() {}

#include "AST/statement/statement.hpp"

StatementNode::StatementNode(const uint32_t line, const uint32_t col) : AstNode{line, col} {}

StatementNode::~StatementNode() {}

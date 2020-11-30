#ifndef __AST_EXPRESSION_BASE_H
#define __AST_EXPRESSION_BASE_H

#include "AST/ast.hpp"

class ExpressionBase : public AstNode {
  public:
    ExpressionBase(const uint32_t line, const uint32_t col);
    ~ExpressionBase();

  protected:
    // for carrying type of result of an expression
    // TODO: for next assignment
};

#endif

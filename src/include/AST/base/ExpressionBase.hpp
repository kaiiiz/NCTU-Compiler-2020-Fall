#ifndef __AST_EXPRESSION_BASE_H
#define __AST_EXPRESSION_BASE_H

#include <memory>

#include "AST/ast.hpp"
#include "type/struct.hpp"

class ExpressionBase : public AstNode {
  public:
    ExpressionBase(const uint32_t line, const uint32_t col);
    ExpressionBase(const uint32_t line, const uint32_t col, std::shared_ptr<TypeStruct> type);
    ~ExpressionBase();

    std::shared_ptr<TypeStruct> getType();

  protected:
    std::shared_ptr<TypeStruct> type;
};

#endif

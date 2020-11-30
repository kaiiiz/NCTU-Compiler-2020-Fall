#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/base/ExpressionBase.hpp"

class VariableReferenceNode : public ExpressionBase {
  public:
    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col
                          /* TODO: name */);
    // array reference
    // VariableReferenceNode(const uint32_t line, const uint32_t col
    //                       /* TODO: name, expressions */);
    ~VariableReferenceNode() = default;

  private:
    // TODO: variable name, expressions
};

#endif

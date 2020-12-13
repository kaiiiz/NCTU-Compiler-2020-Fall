#ifndef __AST_CONSTANT_REAL_VALUE_NODE_H
#define __AST_CONSTANT_REAL_VALUE_NODE_H

#include <memory>

#include "AST/expression/ConstantValue.hpp"

class ConstRealValueNode : public ConstantValueNode {
  public:
    ConstRealValueNode(const uint32_t line, const uint32_t col, double rval);
    ~ConstRealValueNode() = default;

    std::string getValueString() override;
    double getValue();

  private:
    double rval;
};

#endif

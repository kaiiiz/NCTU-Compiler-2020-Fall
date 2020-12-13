#ifndef __AST_CONSTANT_BOOL_VALUE_NODE_H
#define __AST_CONSTANT_BOOL_VALUE_NODE_H

#include <memory>

#include "AST/expression/ConstantValue.hpp"

class ConstBoolValueNode : public ConstantValueNode {
  public:
    ConstBoolValueNode(const uint32_t line, const uint32_t col, bool bval);
    ~ConstBoolValueNode() = default;

    std::string getValueString() override;
    bool getValue();

  private:
    bool bval;
};

#endif

#ifndef __AST_CONSTANT_INT_VALUE_NODE_H
#define __AST_CONSTANT_INT_VALUE_NODE_H

#include <memory>

#include "AST/expression/ConstantValue.hpp"

class ConstIntValueNode : public ConstantValueNode {
  public:
    ConstIntValueNode(const uint32_t line, const uint32_t col, int64_t ival);
    ~ConstIntValueNode() = default;

    std::string getValueString() override;
    int64_t getValue();

  private:
    int64_t ival;
};

#endif

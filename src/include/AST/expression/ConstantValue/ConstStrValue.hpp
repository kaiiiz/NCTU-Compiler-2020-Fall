#ifndef __AST_CONSTANT_STR_VALUE_NODE_H
#define __AST_CONSTANT_STR_VALUE_NODE_H

#include <memory>

#include "AST/expression/ConstantValue.hpp"

class ConstStrValueNode : public ConstantValueNode {
  public:
    ConstStrValueNode(const uint32_t line, const uint32_t col, std::string sval);
    ~ConstStrValueNode() = default;

    std::string getValueString() override;
    std::string getValue();

  private:
    std::string sval;
};

#endif

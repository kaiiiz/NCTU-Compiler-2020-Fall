#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include <vector>
#include <string>
#include <memory>

#include "AST/ast.hpp"

class BaseType;
class ArrayType;
class VariableNode;

class DeclNode : public AstNode {
  public:
    DeclNode(const uint32_t line, const uint32_t col,
             std::vector<std::shared_ptr<VariableNode>> var_list,
             std::shared_ptr<BaseType> type);

    ~DeclNode() = default;

    std::string getType();
    int32_t getVarNum();

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::vector<std::shared_ptr<VariableNode>> var_list;
    std::shared_ptr<BaseType> type;
};

#endif

#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include <string>
#include <vector>

#include "AST/ast.hpp"
#include "type/base.hpp"

class DeclNode;
class CompoundStatementNode;

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col,
                 std::string name, std::vector<std::shared_ptr<DeclNode>> parameters,
                 std::shared_ptr<BaseType> return_type);
    FunctionNode(const uint32_t line, const uint32_t col,
                 std::string name, std::vector<std::shared_ptr<DeclNode>> parameters,
                 std::shared_ptr<BaseType> return_type,
                 std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~FunctionNode() = default;

    std::string getName();
    std::string getFuncProtoType();

    void dump(AstDumper &dp) override;
    void dumpChildNodes(AstDumper &dp) override;

  private:
    std::string name;
    std::vector<std::shared_ptr<DeclNode>> parameters;
    std::shared_ptr<BaseType> return_type;
    std::shared_ptr<CompoundStatementNode> compound_stmt;
};

#endif

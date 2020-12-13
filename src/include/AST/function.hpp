#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include <string>
#include <vector>
#include <memory>

#include "AST/ast.hpp"
#include "type/base.hpp"

class DeclNode;
class CompoundStatementNode;

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col,
                 std::string name, std::vector<std::shared_ptr<DeclNode>> parameters,
                 std::shared_ptr<TypeBase> return_type);
    FunctionNode(const uint32_t line, const uint32_t col,
                 std::string name, std::vector<std::shared_ptr<DeclNode>> parameters,
                 std::shared_ptr<TypeBase> return_type,
                 std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~FunctionNode() = default;

    std::string getNameStr();
    std::string getFuncProtoType();
    std::shared_ptr<TypeBase> getRetType();
    std::vector<std::shared_ptr<TypeBase>> getParamTypeList();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::string name;
    std::vector<std::shared_ptr<DeclNode>> parameters;
    std::shared_ptr<TypeBase> return_type;
    std::shared_ptr<CompoundStatementNode> compound_stmt;
};

#endif

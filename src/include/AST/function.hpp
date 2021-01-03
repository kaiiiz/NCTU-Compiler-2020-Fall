#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include <string>
#include <vector>
#include <memory>

#include "AST/ast.hpp"
#include "AST/base/AstNodeWithSymTab.hpp"
#include "type/struct.hpp"

class DeclNode;
class CompoundStatementNode;

class FunctionNode : public AstNode, public AstNodeWithSymTab {
  public:
    FunctionNode(const uint32_t line, const uint32_t col,
                 std::string name, const std::vector<std::shared_ptr<DeclNode>> parameters,
                 const std::shared_ptr<TypeStruct> return_type);
    FunctionNode(const uint32_t line, const uint32_t col,
                 std::string name, const std::vector<std::shared_ptr<DeclNode>> parameters,
                 const std::shared_ptr<TypeStruct> return_type,
                 const std::shared_ptr<CompoundStatementNode> compound_stmt);
    ~FunctionNode() = default;

    std::string getNameStr();
    std::string getFuncProtoType();
    std::shared_ptr<TypeStruct> getRetType();
    std::vector<std::shared_ptr<TypeStruct>> getParamTypeList();

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

    const std::vector<std::shared_ptr<DeclNode>> parameters;
    const std::shared_ptr<TypeStruct> return_type;
    const std::shared_ptr<CompoundStatementNode> compound_stmt;

  private:
    std::string name;
};

#endif

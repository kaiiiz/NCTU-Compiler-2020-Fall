#ifndef __VISITOR_AST_NODE_INCLUDE_H
#define __VISITOR_AST_NODE_INCLUDE_H

#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/function.hpp"
#include "AST/base/ExpressionBase.hpp"
#include "AST/base/StatementBase.hpp"
#include "AST/expression/ConstantValue.hpp"
#include "AST/expression/BinaryOperator.hpp"
#include "AST/expression/UnaryOperator.hpp"
#include "AST/expression/FunctionInvocation.hpp"
#include "AST/expression/VariableReference.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/statement/print.hpp"
#include "AST/statement/assignment.hpp"
#include "AST/statement/read.hpp"
#include "AST/statement/if.hpp"
#include "AST/statement/while.hpp"
#include "AST/statement/for.hpp"
#include "AST/statement/return.hpp"
#include "AST/statement/FunctionCall.hpp"

#endif

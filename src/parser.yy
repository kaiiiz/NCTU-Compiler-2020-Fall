%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"

%locations
%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires { // dependency code required for YYSTYPE and YYLTYPE
#include <vector>
#include <memory>

#include "type/scalar.hpp"
#include "type/base.hpp"
#include "type/array.hpp"
#include "type/void.hpp"

#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/function.hpp"
#include "AST/statement/CompoundStatement.hpp"
#include "AST/statement/print.hpp"
#include "AST/expression.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/statement/FunctionInvocation.hpp"
#include "AST/VariableReference.hpp"
#include "AST/statement/assignment.hpp"
#include "AST/statement/read.hpp"
#include "AST/statement/if.hpp"
#include "AST/statement/while.hpp"
#include "AST/statement/for.hpp"
#include "AST/statement/return.hpp"

class driver;
}

%param { driver& drv }
%initial-action {
    @$.begin.filename = @$.end.filename = &drv.file; // Initialize the initial location.
};

%code {
#include "driver/driver.hpp"
#include "AST/AstDumper.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>

/* Declared by scanner.l */
extern char *yytext;

template <typename T>
std::vector<std::shared_ptr<VariableNode>> idList2VarNodeList(
    std::vector<std::shared_ptr<IdNode>> idList, std::shared_ptr<T> type) {

    std::vector<std::shared_ptr<VariableNode>> var_list;
    for (auto &idnode : idList) {
        var_list.push_back(std::make_shared<VariableNode>(idnode, type));
    }
    return var_list;
}

}

%token EOF_ 0 "end of file";

    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token ASSIGN
%left OR
%left AND
%right NOT
%left LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right UNARY_MINUS

    /* Keyword */
%token ARRAY BOOLEAN INTEGER REAL
%token STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token FALSE TRUE
%token PRINT READ

    /* Identifier */
%token <std::string> ID "id"

    /* Literal */
%token <int64_t> INT_LITERAL
%token <double> REAL_LITERAL
%token <std::string> STRING_LITERAL


%type <std::string> ProgramName;
%type <std::string> FunctionName;
%type <std::vector<std::shared_ptr<IdNode>>> IdList;
%type <std::shared_ptr<DeclNode>> Declaration;
%type <std::shared_ptr<DeclNode>> FormalArg;
%type <std::vector<std::shared_ptr<DeclNode>>> FormalArgs;
%type <std::vector<std::shared_ptr<DeclNode>>> FormalArgList;
%type <std::vector<std::shared_ptr<DeclNode>>> Declarations;
%type <std::vector<std::shared_ptr<DeclNode>>> DeclarationList;
%type <std::shared_ptr<BaseType>> Type;
%type <std::shared_ptr<BaseType>> ReturnType;
%type <std::shared_ptr<ScalarType>> ScalarType;
%type <std::vector<int64_t>> ArrDecl;
%type <std::shared_ptr<ArrayType>> ArrType;
%type <bool> NegOrNot;
%type <std::shared_ptr<ConstantValueNode>> StringAndBoolean;
%type <std::shared_ptr<ConstantValueNode>> LiteralConstant;
%type <std::shared_ptr<FunctionNode>> FunctionDeclaration;
%type <std::shared_ptr<FunctionNode>> Function;
%type <std::vector<std::shared_ptr<FunctionNode>>> Functions;
%type <std::vector<std::shared_ptr<FunctionNode>>> FunctionList;
%type <std::shared_ptr<CompoundStatementNode>> CompoundStatement;
%type <std::vector<std::shared_ptr<StatementNode>>> StatementList;
%type <std::vector<std::shared_ptr<StatementNode>>> Statements;
%type <std::shared_ptr<StatementNode>> Statement;

%%
    /*
       Program Units
                     */

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        drv.root = std::make_shared<ProgramNode>(@1.begin.line, @1.begin.column, $1, "void", $3, $4, $5);
    }
;

ProgramName:
    ID
;

DeclarationList:
    Epsilon { $$ = std::vector<std::shared_ptr<DeclNode>>(); }
    |
    Declarations { $$ = $1; }
;

Declarations:
    Declaration { $$ = std::vector<std::shared_ptr<DeclNode>>{$1}; }
    |
    Declarations Declaration { $1.push_back($2); $$ = $1; }
;

FunctionList:
    Epsilon { $$ = std::vector<std::shared_ptr<FunctionNode>>(); }
    |
    Functions { $$ = $1; }
;

Functions:
    Function { $$ = std::vector<std::shared_ptr<FunctionNode>>{$1}; }
    |
    Functions Function { $1.push_back($2); $$ = $1; }
;

Function:
    FunctionDeclaration { $$ = $1; }
    |
    FunctionDefinition { $$ = nullptr; }
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON {
        $$ = std::make_shared<FunctionNode>(@1.begin.line, @1.begin.column, $1, $3, $5);
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END
;

FunctionName:
    ID
;

FormalArgList:
    Epsilon { $$ = std::vector<std::shared_ptr<DeclNode>>(); }
    |
    FormalArgs { $$ = $1; }
;

FormalArgs:
    FormalArg { $$ = std::vector<std::shared_ptr<DeclNode>>{$1}; }
    |
    FormalArgs SEMICOLON FormalArg { $1.push_back($3); $$ = $1; }
;

FormalArg:
    IdList COLON Type {
        $$ = std::make_shared<DeclNode>(@$.begin.line, @$.begin.column, idList2VarNodeList<BaseType>($1, $3), $3);
    }
;

IdList:
    ID {
        std::shared_ptr<IdNode> i = std::make_shared<IdNode>(@1.begin.line, @1.begin.column, $1);
        $$ = std::vector<std::shared_ptr<IdNode>>{i};
    }
    |
    IdList COMMA ID {
        std::shared_ptr<IdNode> i = std::make_shared<IdNode>(@3.begin.line, @3.begin.column, $3);
        $1.push_back(i);
        $$ = $1;
    }
;

ReturnType:
    COLON ScalarType { $$ = std::dynamic_pointer_cast<BaseType>($2); }
    |
    Epsilon { $$ = std::dynamic_pointer_cast<BaseType>(std::make_shared<VoidType>()); }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        $$ = std::make_shared<DeclNode>(@1.begin.line, @1.begin.column,
                            idList2VarNodeList<BaseType>($2, $4), $4);
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        $$ = std::make_shared<DeclNode>(@1.begin.line, @1.begin.column,
                            idList2VarNodeList<ConstantValueNode>($2, $4),
                            std::make_shared<ScalarType>($4->getType()));
    }
;

Type:
    ScalarType { $$ = std::dynamic_pointer_cast<BaseType>($1); }
    |
    ArrType { $$ = std::dynamic_pointer_cast<BaseType>($1); }
;

ScalarType:
    INTEGER { $$ = std::make_shared<ScalarType>(scalar_type_t::integer); }
    |
    REAL { $$ = std::make_shared<ScalarType>(scalar_type_t::real); }
    |
    STRING { $$ = std::make_shared<ScalarType>(scalar_type_t::string); }
    |
    BOOLEAN { $$ = std::make_shared<ScalarType>(scalar_type_t::boolean); }
;

ArrType:
    ArrDecl ScalarType { $$ = std::make_shared<ArrayType>($2->getType(), $1); }
;

ArrDecl:
    ARRAY INT_LITERAL OF { $$ = std::vector<int64_t>{$2}; }
    |
    ArrDecl ARRAY INT_LITERAL OF { $1.push_back($3); $$ = $1; }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        $$ = ($1) ? 
            std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, scalar_type_t::integer, $2 * (-1))
            :
            std::make_shared<ConstantValueNode>(@2.begin.line, @2.begin.column, scalar_type_t::integer, $2);
    }
    |
    NegOrNot REAL_LITERAL {
        $$ = ($1) ? 
            std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, scalar_type_t::real, $2 * (-1))
            :
            std::make_shared<ConstantValueNode>(@2.begin.line, @2.begin.column, scalar_type_t::real, $2);
    }
    |
    StringAndBoolean { $$ = $1; }
;

NegOrNot:
    Epsilon { $$ = false; }
    |
    MINUS %prec UNARY_MINUS { $$ = true; }
;

StringAndBoolean:
    STRING_LITERAL { $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, scalar_type_t::string, $1); }
    |
    TRUE { $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, scalar_type_t::boolean, true); }
    |
    FALSE { $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, scalar_type_t::boolean, false); }
;

IntegerAndReal:
    INT_LITERAL
    |
    REAL_LITERAL
;

    /*
       Statements
                  */

Statement:
    CompoundStatement { $$ = std::dynamic_pointer_cast<StatementNode>($1); }
    |
    Simple { $$ = nullptr; }
    |
    Condition { $$ = nullptr; }
    |
    While { $$ = nullptr; }
    |
    For { $$ = nullptr; }
    |
    Return { $$ = nullptr; }
    |
    FunctionCall { $$ = nullptr; }
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END {
        $$ = std::make_shared<CompoundStatementNode>(@1.begin.line, @1.begin.column, $2, $3);
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON
    |
    PRINT Expression SEMICOLON
    |
    READ VariableReference SEMICOLON
;

VariableReference:
    ID ArrRefList
;

ArrRefList:
    Epsilon
    |
    ArrRefs
;

ArrRefs:
    L_BRACKET Expression R_BRACKET
    |
    ArrRefs L_BRACKET Expression R_BRACKET
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF
;

ElseOrNot:
    ELSE
    CompoundStatement
    |
    Epsilon
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO
;

Return:
    RETURN Expression SEMICOLON
;

FunctionCall:
    FunctionInvocation SEMICOLON
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS
;

ExpressionList:
    Epsilon
    |
    Expressions
;

Expressions:
    Expression
    |
    Expressions COMMA Expression
;

StatementList:
    Epsilon { $$ = std::vector<std::shared_ptr<StatementNode>>(); }
    |
    Statements { $$ = $1; }
;

Statements:
    Statement { $$ = std::vector<std::shared_ptr<StatementNode>>{$1}; }
    |
    Statements Statement { $1.push_back($2); $$ = $1; }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS
    |
    MINUS Expression %prec UNARY_MINUS
    |
    Expression MULTIPLY Expression
    |
    Expression DIVIDE Expression
    |
    Expression MOD Expression
    |
    Expression PLUS Expression
    |
    Expression MINUS Expression
    |
    Expression LESS Expression
    |
    Expression LESS_OR_EQUAL Expression
    |
    Expression GREATER Expression
    |
    Expression GREATER_OR_EQUAL Expression
    |
    Expression EQUAL Expression
    |
    Expression NOT_EQUAL Expression
    |
    NOT Expression
    |
    Expression AND Expression
    |
    Expression OR Expression
    |
    IntegerAndReal
    |
    StringAndBoolean
    |
    VariableReference
    |
    FunctionInvocation
;

    /*
       misc
            */
Epsilon:
;
%%

void yy::parser::error(const location_type &l, const std::string &m) {
    drv.error(l, m, yytext);
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./parser <filename> [--dump-ast]\n");
        exit(-1);
    }

    driver drv;
    drv.parse(argv[1]);

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        AstDumper dp;
        drv.root->dump(dp);
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    return 0;
}

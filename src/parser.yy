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

#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/print.hpp"
#include "AST/expression.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/read.hpp"
#include "AST/if.hpp"
#include "AST/while.hpp"
#include "AST/for.hpp"
#include "AST/return.hpp"

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
%token REAL_LITERAL
%token STRING_LITERAL


%type <std::string> ProgramName;
%type <std::vector<std::shared_ptr<IdNode>>> IdList;
%type <std::shared_ptr<DeclNode>> Declaration;
%type <std::vector<std::shared_ptr<DeclNode>>> Declarations;
%type <std::vector<std::shared_ptr<DeclNode>>> DeclarationList;
%type <std::shared_ptr<BaseType>> Type;
%type <std::shared_ptr<ScalarType>> ScalarType;
%type <std::vector<int64_t>> ArrDecl;
%type <std::shared_ptr<ArrayType>> ArrType;

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
        std::vector<std::shared_ptr<FunctionNode>> func_list;
        std::shared_ptr<CompoundStatementNode> compound_stmt;
        drv.root = std::make_shared<ProgramNode>(@1.begin.line, @1.begin.column, $1, "void", $3, func_list, compound_stmt);
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
    Epsilon
    |
    Functions
;

Functions:
    Function
    |
    Functions Function
;

Function:
    FunctionDeclaration
    |
    FunctionDefinition
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON
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
    Epsilon
    |
    FormalArgs
;

FormalArgs:
    FormalArg
    |
    FormalArgs SEMICOLON FormalArg
;

FormalArg:
    IdList COLON Type
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
    COLON ScalarType
    |
    Epsilon
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        std::vector<std::shared_ptr<VariableNode>> var_list;
        for (auto &idnode : $2) {
            var_list.push_back(std::make_shared<VariableNode>(idnode, $4));
        }
        $$ = std::make_shared<DeclNode>(@1.begin.line, @1.begin.column, var_list);
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON { $$ = nullptr; }
;

Type:
    ScalarType { $$ = std::dynamic_pointer_cast<BaseType>($1); }
    |
    ArrType { $$ = std::dynamic_pointer_cast<BaseType>($1); }
;

ScalarType:
    INTEGER { $$ = std::make_shared<ScalarType>("integer"); }
    |
    REAL { $$ = std::make_shared<ScalarType>("real"); }
    |
    STRING { $$ = std::make_shared<ScalarType>("string"); }
    |
    BOOLEAN { $$ = std::make_shared<ScalarType>("boolean"); }
;

ArrType:
    ArrDecl ScalarType { $$ = std::make_shared<ArrayType>($2->getTypeName(), $1); }
;

ArrDecl:
    ARRAY INT_LITERAL OF { $$ = std::vector<int64_t>{$2}; }
    |
    ArrDecl ARRAY INT_LITERAL OF { $1.push_back($3); $$ = $1; }
;

LiteralConstant:
    NegOrNot INT_LITERAL
    |
    NegOrNot REAL_LITERAL
    |
    StringAndBoolean
;

NegOrNot:
    Epsilon
    |
    MINUS %prec UNARY_MINUS
;

StringAndBoolean:
    STRING_LITERAL
    |
    TRUE
    |
    FALSE
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
    CompoundStatement
    |
    Simple
    |
    Condition
    |
    While
    |
    For
    |
    Return
    |
    FunctionCall
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END
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
    Epsilon
    |
    Statements
;

Statements:
    Statement
    |
    Statements Statement
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

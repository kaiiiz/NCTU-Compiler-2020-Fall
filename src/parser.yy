%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"

%locations
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires { // dependency code required for YYSTYPE and YYLTYPE
#include <vector>
#include <memory>

#include "type/struct.hpp"
#include "visitor/AstNodeInclude.hpp"

class driver;
}

%param { driver& drv }

%code {
#include "driver/driver.hpp"
#include "visitor/AstDumper.hpp"
#include "visitor/SemanticAnalyzer.hpp"
#include "visitor/CodeGenerator.hpp"

#include "type/manager.hpp"
#include "sema/SemaInclude.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>

/* Declared by scanner.l */
extern char *yytext;
extern TypeManager type_mgr;
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


%type <std::string> ProgramName FunctionName;
%type <bool> NegOrNot;
%type <std::vector<std::shared_ptr<VariableNode>>> IdList;
%type <std::shared_ptr<DeclNode>> Declaration FormalArg;
%type <std::vector<std::shared_ptr<DeclNode>>> FormalArgs FormalArgList Declarations DeclarationList;
%type <std::shared_ptr<TypeStruct>> Type ArrType ReturnType ScalarType;
%type <std::vector<int64_t>> ArrDecl;
%type <std::shared_ptr<ConstantValueNode>> IntegerAndReal StringAndBoolean LiteralConstant;
%type <std::shared_ptr<FunctionNode>> FunctionDeclaration FunctionDefinition Function;
%type <std::vector<std::shared_ptr<FunctionNode>>> Functions FunctionList;
%type <std::shared_ptr<CompoundStatementNode>> CompoundStatement ElseOrNot;
%type <std::vector<std::shared_ptr<StatementBase>>> StatementList Statements;
%type <std::shared_ptr<StatementBase>> Statement Simple;
%type <std::shared_ptr<ExpressionBase>> Expression;
%type <std::vector<std::shared_ptr<ExpressionBase>>> ExpressionList Expressions ArrRefList ArrRefs;
%type <std::shared_ptr<FunctionInvocationNode>> FunctionInvocation;
%type <std::shared_ptr<FunctionCallNode>> FunctionCall;
%type <std::shared_ptr<VariableReferenceNode>> VariableReference;
%type <std::shared_ptr<IfNode>> Condition;
%type <std::shared_ptr<WhileNode>> While;
%type <std::shared_ptr<ForNode>> For;
%type <std::shared_ptr<ReturnNode>> Return;

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
        $5->fillAttribute(CompoundKind::normal);
        drv.root = std::make_shared<ProgramNode>(@1.begin.line, @1.begin.column, $1, type_mgr.getType(TypeKind::Void), $3, $4, $5);
    }
;

ProgramName:
    ID { $$ = $1; }
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
    FunctionDefinition { $$ = $1; }
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON {
        $$ = std::make_shared<FunctionNode>(@1.begin.line, @1.begin.column, $1, $3, $5);
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END {
        $6->fillAttribute(CompoundKind::function);
        $$ = std::make_shared<FunctionNode>(@1.begin.line, @1.begin.column, $1, $3, $5, $6);
    }
;

FunctionName:
    ID { $$ = $1; }
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
        for (auto &var : $1) {
            var->fillAttribute($3);
            var->fillAttribute(VariableKind::Parameter);
        }
        $$ = std::make_shared<DeclNode>(@$.begin.line, @$.begin.column, $1, $3);
    }
;

IdList:
    ID {
        auto i = std::make_shared<VariableNode>(@1.begin.line, @1.begin.column, $1);
        $$ = std::vector<std::shared_ptr<VariableNode>>{i};
    }
    |
    IdList COMMA ID {
        $1.push_back(std::make_shared<VariableNode>(@3.begin.line, @3.begin.column, $3));
        $$ = $1;
    }
;

ReturnType:
    COLON ScalarType { $$ = $2; }
    |
    Epsilon { $$ = type_mgr.getType(TypeKind::Void); }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON {
        for (auto &var : $2) {
            var->fillAttribute($4);
            var->fillAttribute(VariableKind::Variable);
        }
        $$ = std::make_shared<DeclNode>(@1.begin.line, @1.begin.column, $2, $4);
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON {
        for (auto &var : $2) {
            var->fillAttribute($4);
            var->fillAttribute(VariableKind::Constant);
        }
        $$ = std::make_shared<DeclNode>(@1.begin.line, @1.begin.column, $2, $4->getType());
    }
;

Type:
    ScalarType { $$ = $1; }
    |
    ArrType { $$ = $1; }
;

ScalarType:
    INTEGER { $$ = type_mgr.getType(TypeKind::Integer); }
    |
    REAL { $$ = type_mgr.getType(TypeKind::Real); }
    |
    STRING { $$ = type_mgr.getType(TypeKind::String); }
    |
    BOOLEAN { $$ = type_mgr.getType(TypeKind::Boolean); }
;

ArrType:
    ArrDecl ScalarType { $$ = type_mgr.getType($2->kind, $1); }
;

ArrDecl:
    ARRAY INT_LITERAL OF { $$ = std::vector<int64_t>{$2}; }
    |
    ArrDecl ARRAY INT_LITERAL OF { $1.push_back($3); $$ = $1; }
;

LiteralConstant:
    NegOrNot INT_LITERAL {
        $$ = ($1) ? 
            std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, $2 * (-1))
            :
            std::make_shared<ConstantValueNode>(@2.begin.line, @2.begin.column, $2);
    }
    |
    NegOrNot REAL_LITERAL {
        $$ = ($1) ? 
            std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, $2 * (-1))
            :
            std::make_shared<ConstantValueNode>(@2.begin.line, @2.begin.column, $2);
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
    STRING_LITERAL {
        $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, $1);
    }
    |
    TRUE {
        $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, true);
    }
    |
    FALSE {
        $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, false);
    }
;

IntegerAndReal:
    INT_LITERAL {
        $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, $1);
    }
    |
    REAL_LITERAL {
        $$ = std::make_shared<ConstantValueNode>(@1.begin.line, @1.begin.column, $1);
    }
;

    /*
       Statements
                  */

Statement:
    CompoundStatement { 
        $1->fillAttribute(CompoundKind::normal);
        $$ = std::dynamic_pointer_cast<StatementBase>($1);
    }
    |
    Simple { $$ = $1; }
    |
    Condition { $$ = std::dynamic_pointer_cast<StatementBase>($1); }
    |
    While { $$ = std::dynamic_pointer_cast<StatementBase>($1); }
    |
    For { $$ = std::dynamic_pointer_cast<StatementBase>($1); }
    |
    Return { $$ = std::dynamic_pointer_cast<StatementBase>($1); }
    |
    FunctionCall { $$ = std::dynamic_pointer_cast<StatementBase>($1); }
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
    VariableReference ASSIGN Expression SEMICOLON {
        $$ = std::dynamic_pointer_cast<StatementBase>(
                std::make_shared<AssignmentNode>(@2.begin.line, @2.begin.column, $1, $3)
             );
    }
    |
    PRINT Expression SEMICOLON {
        $$ = std::dynamic_pointer_cast<StatementBase>(
                std::make_shared<PrintNode>(@1.begin.line, @1.begin.column, $2)
             );
    }
    |
    READ VariableReference SEMICOLON {
        $$ = std::dynamic_pointer_cast<StatementBase>(
                std::make_shared<ReadNode>(@1.begin.line, @1.begin.column, $2)
             );
    }
;

VariableReference:
    ID ArrRefList { $$ = std::make_shared<VariableReferenceNode>(@1.begin.line, @1.begin.column, $1, $2); }
;

ArrRefList:
    Epsilon { $$ = std::vector<std::shared_ptr<ExpressionBase>>(); }
    |
    ArrRefs { $$ = $1; }
;

ArrRefs:
    L_BRACKET Expression R_BRACKET { $$ = std::vector<std::shared_ptr<ExpressionBase>>{$2}; }
    |
    ArrRefs L_BRACKET Expression R_BRACKET { $1.push_back($3); $$ = $1; }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF {
        $4->fillAttribute(CompoundKind::normal);
        $$ = std::make_shared<IfNode>(@1.begin.line, @1.begin.column, $2, $4, $5);
    }
;

ElseOrNot:
    ELSE
    CompoundStatement { 
        $2->fillAttribute(CompoundKind::normal);
        $$ = $2;
    }
    |
    Epsilon { $$ = nullptr; }
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO {
        $4->fillAttribute(CompoundKind::normal);
        $$ = std::make_shared<WhileNode>(@1.begin.line, @1.begin.column, $2, $4);
    }
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO {
        // make declaration
        auto type = type_mgr.getType(TypeKind::Integer);
        auto var = std::make_shared<VariableNode>(@2.begin.line, @2.begin.column, $2, type);
        var->fillAttribute(VariableKind::LoopVar);
        auto var_list = std::vector<std::shared_ptr<VariableNode>>{var};
        auto decl = std::make_shared<DeclNode>(@2.begin.line, @2.begin.column, var_list, type);
        // make assignment
        auto constant_init = std::make_shared<ConstantValueNode>(@4.begin.line, @4.begin.column, $4);
        auto var_ref_indices = std::vector<std::shared_ptr<ExpressionBase>>();
        auto var_ref = std::make_shared<VariableReferenceNode>(@2.begin.line, @2.begin.column, $2, var_ref_indices);
        auto assignment = std::make_shared<AssignmentNode>(@3.begin.line, @3.begin.column, var_ref, constant_init);
        // make condition
        auto condition = std::make_shared<ConstantValueNode>(@6.begin.line, @6.begin.column, $6);
        $8->fillAttribute(CompoundKind::normal);
        $$ = std::make_shared<ForNode>(@1.begin.line, @1.begin.column, decl, assignment, condition, $8);
    }
;

Return:
    RETURN Expression SEMICOLON { $$ = std::make_shared<ReturnNode>(@1.begin.line, @1.begin.column, $2); }
;

FunctionCall:
    FunctionInvocation SEMICOLON {
        $$ = std::make_shared<FunctionCallNode>(@1.begin.line, @1.begin.column, $1);
    }
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS {
        $$ = std::make_shared<FunctionInvocationNode>(@1.begin.line, @1.begin.column, $1, $3);
    }
;

ExpressionList:
    Epsilon { $$ = std::vector<std::shared_ptr<ExpressionBase>>(); }
    |
    Expressions { $$ = $1; }
;

Expressions:
    Expression { $$ = std::vector<std::shared_ptr<ExpressionBase>>{$1}; }
    |
    Expressions COMMA Expression { $1.push_back($3); $$ = $1; }
;

StatementList:
    Epsilon { $$ = std::vector<std::shared_ptr<StatementBase>>(); }
    |
    Statements { $$ = $1; }
;

Statements:
    Statement { $$ = std::vector<std::shared_ptr<StatementBase>>{$1}; }
    |
    Statements Statement { $1.push_back($2); $$ = $1; }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS { $$ = $2; }
    |
    MINUS Expression %prec UNARY_MINUS {
        $$ = std::make_shared<UnaryOperatorNode>(@1.begin.line, @1.begin.column, UnaryOP::MINUS, $2);
    }
    |
    Expression MULTIPLY Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::MULTIPLY, $1, $3);
    }
    |
    Expression DIVIDE Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::DIVIDE, $1, $3);
    }
    |
    Expression MOD Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::MOD, $1, $3);
    }
    |
    Expression PLUS Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::PLUS, $1, $3);
    }
    |
    Expression MINUS Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::MINUS, $1, $3);
    }
    |
    Expression LESS Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::LESS, $1, $3);
    }
    |
    Expression LESS_OR_EQUAL Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::LESS_OR_EQUAL, $1, $3);
    }
    |
    Expression GREATER Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::GREATER, $1, $3);
    }
    |
    Expression GREATER_OR_EQUAL Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::GREATER_OR_EQUAL, $1, $3);
    }
    |
    Expression EQUAL Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::EQUAL, $1, $3);
    }
    |
    Expression NOT_EQUAL Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::NOT_EQUAL, $1, $3);
    }
    |
    NOT Expression {
        $$ = std::make_shared<UnaryOperatorNode>(@1.begin.line, @1.begin.column, UnaryOP::NOT, $2);
    }
    |
    Expression AND Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::AND, $1, $3);
    }
    |
    Expression OR Expression {
        $$ = std::make_shared<BinaryOperatorNode>(@2.begin.line, @2.begin.column, BinaryOP::OR, $1, $3);
    }
    |
    IntegerAndReal { $$ = std::dynamic_pointer_cast<ExpressionBase>($1); }
    |
    StringAndBoolean { $$ = std::dynamic_pointer_cast<ExpressionBase>($1); }
    |
    VariableReference { $$ = std::dynamic_pointer_cast<ExpressionBase>($1); }
    |
    FunctionInvocation { $$ = std::dynamic_pointer_cast<ExpressionBase>($1); }
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
    driver drv;
    drv.parse_cmd_args(argc, argv);
    drv.parse(drv.in_file_path);

    if (drv.opt_dump_ast) {
        AstDumper dp;
        drv.root->accept(dp);
    }
    SemanticAnalyzer analyzer(drv.symbol_mgr, drv.line_head, drv.in_file_path);
    drv.root->accept(analyzer);
    if (drv.opt_dump_symtab) {
        drv.symbol_mgr.dumpSymTab();
    }

    if (!analyzer.hasError()) {
        printf("\n"
            "|---------------------------------------------------|\n"
            "|  There is no syntactic error and semantic error!  |\n"
            "|---------------------------------------------------|\n");
        CodeGenerator cg(drv.symbol_mgr, drv.in_file_name, drv.out_file_path);
        drv.root->accept(cg);
    }

    return 0;
}

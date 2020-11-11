%{
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int32_t line_num;   /* declared in scanner.l */
extern char buffer[512];  /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void); 
static void yyerror(const char *msg);
%}

%token DELIM_SCLN
%token DELIM_CLN
%token DELIM_COMMA
%token DELIM_PTHS_L
%token DELIM_PTHS_R
%token DELIM_BRKT_L
%token DELIM_BRKT_R
%token OP_NEG
%token OP_ASSIGN
%token KW_DEF
%token KW_DO
%token KW_FOR
%token KW_TO
%token KW_RET
%token KW_WHILE
%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_TRUE
%token KW_FALSE
%token KW_END
%token KW_SCALAR_T
%token KW_ARR
%token KW_VAR
%token KW_OF
%token KW_BEGIN
%token KW_PRINT
%token KW_READ
%token ID
%token SCI
%token FLOAT
%token DEC_INT
%token OCT_INT
%token STR

%%

program         : ID DELIM_SCLN vc_decl_list_ep f_decl_list_ep compound_stmt KW_END
                ;
vc_decl_list_ep : epsilon
                | vc_decl_list
                ;
vc_decl_list    : vc_decl
                | vc_decl_list vc_decl
                ;
vc_decl         : var_decl
                | const_decl
                ;
f_decl_list_ep  : epsilon
                | f_decl_list
                ;
f_decl_list     : func_decl
                | f_decl_list func_decl
                ;
func_decl       : ID DELIM_PTHS_L arg_list_ep DELIM_PTHS_R DELIM_CLN KW_SCALAR_T DELIM_SCLN
                | ID DELIM_PTHS_L arg_list_ep DELIM_PTHS_R DELIM_SCLN
                ;
arg_list_ep     : epsilon
                | arg_list
                ;
arg_list        : arg
                | arg_list DELIM_SCLN arg
                ;
arg             : id_list DELIM_CLN KW_SCALAR_T
                ;
var_decl        : KW_VAR id_list DELIM_CLN type DELIM_SCLN
                ;
type            : KW_SCALAR_T
                | KW_ARR int_const KW_OF type
                ;
const_decl      : KW_VAR id_list DELIM_CLN literal_const DELIM_SCLN
                ;
compound_stmt   : KW_BEGIN vc_decl_list_ep stmt_list_ep KW_END
                ;
stmt_list_ep    : epsilon
                | stmt_list
                ;
stmt_list       : stmt
                | stmt_list stmt
                ;
stmt            : simple_stmt
                | cond_stmt
                | while_stmt
                | for_stmt
                | return_stmt
                | call_stmt
                ;
simple_stmt     : var_ref OP_ASSIGN expr DELIM_SCLN
                | KW_PRINT var_ref DELIM_SCLN
                | KW_PRINT expr DELIM_SCLN
                | KW_READ var_ref DELIM_SCLN
                ;
cond_stmt       : KW_IF expr KW_THEN compound_stmt KW_ELSE compound_stmt KW_END KW_IF
                | KW_IF expr KW_THEN compound_stmt KW_END KW_IF
                ;
while_stmt      : KW_WHILE expr KW_DO compound_stmt KW_END KW_DO
                ;
for_stmt        : KW_FOR ID OP_ASSIGN int_const KW_TO int_const KW_DO compound_stmt KW_END KW_DO
                ;
return_stmt     : KW_RET expr DELIM_SCLN
                ;
call_stmt       : ID DELIM_PTHS_L expr_list_ep DELIM_PTHS_R DELIM_SCLN
                ;
var_ref         : ID
                | ID arr_ref_list
                ;
arr_ref_list    : arr_ref
                | arr_ref_list arr_ref
                ;
arr_ref         : DELIM_BRKT_L expr DELIM_BRKT_R
                ;
expr_list_ep    : epsilon
                | expr_list
                ;
expr_list       : expr
                | expr_list DELIM_COMMA expr
                ;
expr            : literal_const
                | var_ref
                ;
id_list         : ID
                | ID DELIM_COMMA id_list
                ;
literal_const   : real_const
                | STR
                | KW_TRUE
                | KW_FALSE
                ;
real_const      : SCI
                | FLOAT
                | OP_NEG SCI
                | OP_NEG FLOAT
                | int_const
                ;
int_const       : DEC_INT
                | OCT_INT
                | OP_NEG DEC_INT
                | OP_NEG OCT_INT
                ;
epsilon         :
                ;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./parser <filename>\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.");

    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}

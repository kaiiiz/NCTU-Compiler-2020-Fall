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

%token DELIM
%token DELIM_SCLN
%token DELIM_CLN
%token DELIM_COMMA
%token OP
%token OP_NEG
%token KW
%token KW_TRUE
%token KW_FALSE
%token KW_END
%token KW_SCALAR_T
%token KW_ARR
%token KW_VAR
%token KW_OF
%token ID
%token SCI
%token FLOAT
%token DEC_INT
%token OCT_INT
%token STR

%%

program         : ID DELIM_SCLN decl_list_ep KW_END
                ;
decl_list_ep    : epsilon
                | decl_list
                ;
decl_list       : decl
                | decl_list decl
                ;
decl            : var_decl
                | const_decl
                ;
var_decl        : KW_VAR id_list DELIM_CLN KW_SCALAR_T DELIM_SCLN
                | KW_VAR id_list DELIM_CLN KW_ARR pos_int_const KW_OF KW_SCALAR_T DELIM_SCLN
                ;
const_decl      : KW_VAR id_list DELIM_CLN literal_const DELIM_SCLN
                ;
id_list         : ID
                | ID DELIM_COMMA id_list
                ;
literal_const   : real_const
                | STR
                | KW_TRUE
                | KW_FALSE
                ;
real_const      : pos_real_const
                | OP_NEG pos_real_const
                ;
pos_real_const  : pos_int_const
                | SCI
                | FLOAT
                ;
pos_int_const   : DEC_INT
                | OCT_INT
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

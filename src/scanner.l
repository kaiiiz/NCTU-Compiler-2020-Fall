%{
#include <stdint.h>
#include "parser.tab.h"

#define LIST                  strCat(yytext)
#define TOKEN(t)              { LIST; if (opt_tok) printf("<%s>\n", t); }
#define TOKEN_CHAR(t)         { LIST; if (opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_KW(s)           { LIST; if (opt_tok) printf("<KW%s>\n", (s)); }
#define TOKEN_STRING(t, s)    { LIST; if (opt_tok) printf("<%s: %s>\n", #t, (s)); }
#define MAX_LINE_LENG         512

uint32_t line_num = 1;
char buffer[MAX_LINE_LENG];

static int32_t opt_src = 1;
static int32_t opt_tok = 1;
static char *buffer_ptr = buffer;

static void strCat(const char *Text);

%}

digit           [0-9]
nonzero_digit   [1-9]
letter          [a-zA-Z]

arithmetic      \+|\*|\/|mod
relational      <|<=|<>|>=|>|=
logical         and|or|not

operators       {arithmetic}|{relational}|{logical}
keywords        def|do|else|for|if|then|to|return|while
scalar_type     integer|real|string|boolean
identifier      {letter}({letter}|{digit})*
positive_int    {nonzero_digit}{digit}*
dec_integer     {positive_int}|0
oct_integer     0[0-7]+
float_frac      {digit}*{nonzero_digit}|0
float           {dec_integer}\.{float_frac}
scientific      ({float}|{dec_integer})[eE][+-]?{dec_integer}
string          \"([\x00-\x09\x0B\x0C\x0E-\x1F\x7F !#-~]|\"\")*\"
whitespace      [[:space:]]
comment_inline  \/\/
comment_blk_beg \/\*
comment_blk_end \*\/
pseudo_commment \/\/&[A-Z][+-]

%x COMMENT_BLK
%x COMMENT_INL

%%

{pseudo_commment} {
    switch (yytext[3]) {
        case 'T':
            opt_tok = (yytext[4] == '+') ? 1 : 0;
            break;
        case 'S':
            opt_src = (yytext[4] == '+') ? 1 : 0;
            break;
    }
    LIST;
    BEGIN(COMMENT_INL);
}

{comment_inline} {
    LIST;
    BEGIN(COMMENT_INL);
}

<COMMENT_INL>. {
    LIST;
}

<COMMENT_INL>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
    BEGIN(INITIAL);
}

{comment_blk_beg} {
    LIST;
    BEGIN(COMMENT_BLK);
}

<COMMENT_BLK>{comment_blk_end} {
    LIST;
    BEGIN(INITIAL);
}

<COMMENT_BLK>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

<COMMENT_BLK>. {
    LIST;
}

 /* DELIMITER */

; {
    TOKEN_CHAR(*yytext);
    return DELIM_SCLN;
}

: {
    TOKEN_CHAR(*yytext);
    return DELIM_CLN;
}

, {
    TOKEN_CHAR(*yytext);
    return DELIM_COMMA;
}

\( {
    TOKEN_CHAR(*yytext);
    return DELIM_PTHS_L;
}

\) {
    TOKEN_CHAR(*yytext);
    return DELIM_PTHS_R;
}

\[ {
    TOKEN_CHAR(*yytext);
    return DELIM_BRKT_L;
}

\] {
    TOKEN_CHAR(*yytext);
    return DELIM_BRKT_R;
}

 /* -------- */

 /* OPERATOR */

{operators} {
    TOKEN(yytext);
    return OP;
}

:= {
    TOKEN(yytext);
    return OP_ASSIGN;
}

\- {
    TOKEN(yytext);
    return OP_NEG;
}

 /* -------- */

 /* KEYWORD */

{keywords} {
    TOKEN_KW(yytext);
    return KW;
}

{scalar_type} {
    TOKEN_KW(yytext);
    return KW_SCALAR_T;
}

read {
    TOKEN_KW(yytext);
    return KW_READ;
}

print {
    TOKEN_KW(yytext);
    return KW_PRINT;
}

true {
    TOKEN_KW(yytext);
    return KW_TRUE;
}

false {
    TOKEN_KW(yytext);
    return KW_FALSE;
}

of {
    TOKEN_KW(yytext);
    return KW_OF;
}

array {
    TOKEN_KW(yytext);
    return KW_ARR;
}

var {
    TOKEN_KW(yytext);
    return KW_VAR;
}

begin {
    TOKEN_KW(yytext);
    return KW_BEGIN;
}

end {
    TOKEN_KW(yytext);
    return KW_END;
}

 /* ------ */

{identifier} {
    TOKEN_STRING(id, yytext);
    return ID;
}

{scientific} {
    TOKEN_STRING(scientific, yytext);
    return SCI;
}

{float} {
    TOKEN_STRING(float, yytext);
    return FLOAT;
}

{dec_integer} {
    TOKEN_STRING(integer, yytext);
    return DEC_INT;
}

{oct_integer} {
    TOKEN_STRING(oct_integer, yytext);
    return OCT_INT;
}

{string} {
    int max_str_len = strlen(yytext) - 2;
    int idx = 0;
    char str[max_str_len + 1];
    char *ptr = yytext + 1;
    while (max_str_len--) {
        if (*ptr == '"' && *(ptr + 1) == '"') {
            ptr++;
            max_str_len--;
        }
        str[idx++] = *ptr++; 
    }
    str[idx] = '\0';
    TOKEN_STRING(string, str);
    return STR;
}

\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

{whitespace} {
    LIST;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void strCat(const char *Text) {
    while (*Text) {
        *buffer_ptr++ = *Text++;
    }
    *buffer_ptr = '\0';
}

/* int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./scanner [filename]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    return 0;
} */

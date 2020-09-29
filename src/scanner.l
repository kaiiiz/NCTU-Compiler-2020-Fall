%{
#include <stdint.h>

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
letter          [_a-zA-Z]

arithmetic      \+|\-|\*|\/|mod
assignment      :=
relational      <|<=|<>|>=|>|=
logical         and|or|not

delimiters      [,;:\(\)\[\]]
operators       {arithmetic}|{assignment}|{relational}|{logical}
keywords        array|begin|boolean|def|do|else|end|false|for|integer|if|of|print|read|real|string|then|to|true|return|var|while
identifier      {letter}({letter}|{digit})*
positive_int    {nonzero_digit}{digit}*
integer         {positive_int}|0
oct_integer     0{digit}+
float_frac      {digit}*{nonzero_digit}|0
float           {integer}\.{float_frac}

%%

{delimiters} {
    TOKEN_CHAR(*yytext);
}

{operators} {
    TOKEN(yytext);
}

{keywords} {
    TOKEN_KW(yytext);
}

{identifier} {
    TOKEN_STRING(id, yytext);
}

 /* For case: one or more 0 before float */
0+{float} {
    char* first_nonzero = yytext;
    while (*first_nonzero == '0') first_nonzero++;
    if (*first_nonzero == '.') first_nonzero--; // 0.xxx

    int zero_len = first_nonzero - yytext;
    yyless(zero_len);

    char zeros[zero_len + 1];
    strncpy(zeros, yytext, zero_len);
    zeros[zero_len] = '\0';

    if (zero_len == 1) {
        TOKEN_STRING(integer, zeros);
    }
    else {
        TOKEN_STRING(oct_integer, zeros);
    }
}

{float} {
    TOKEN_STRING(float, yytext);
}

{integer} {
    TOKEN_STRING(integer, yytext);
}

{oct_integer} {
    TOKEN_STRING(oct_integer, yytext);
}

\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
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

int main(int argc, const char **argv) {
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
}

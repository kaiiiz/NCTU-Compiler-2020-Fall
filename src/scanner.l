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

digit       [0-9]
letter      [_a-zA-Z]

arithmetic  \+|\-|\*|\/|mod
assignment  :=
relational  <|<=|<>|>=|>|=
logical     and|or|not

delimiters  [,;:\(\)\[\]]
operators   {arithmetic}|{assignment}|{relational}|{logical}
keywords    array|begin|boolean|def|do|else|end|false|for|integer|if|of|print|read|real|string|then|to|true|return|var|while
identifier  {letter}({letter}|{digit})*

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

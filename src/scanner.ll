%{ /* -*- C++ -*- */
#include <string>

#include "parser.hh"
#include "driver/driver.hpp"

#define LIST                drv.scan_list(yytext)
#define TOKEN(t)            { LIST; if (drv.opt_tok) printf("<%s>\n", #t); }
#define TOKEN_CHAR(t)       { LIST; if (drv.opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (drv.opt_tok) printf("<%s: %s>\n", #t, (s)); }

extern driver drv;
%}

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION      loc.columns(yyleng);
%}

%option noyywrap nounput noinput

integer 0|[1-9][0-9]*
float {integer}\.(0|[0-9]*[1-9])

%x CCOMMENT

%%

%{
  // A handy shortcut to the location held by the driver.
  yy::location &loc = drv.location;
  // Code run each time yylex is called.
  loc.step();
%}

    /* Delimiter */
"," { TOKEN_CHAR(','); return yy::parser::make_COMMA(loc); }
";" { TOKEN_CHAR(';'); return yy::parser::make_SEMICOLON(loc); }
":" { TOKEN_CHAR(':'); return yy::parser::make_COLON(loc); }
"(" { TOKEN_CHAR('('); return yy::parser::make_L_PARENTHESIS(loc); }
")" { TOKEN_CHAR(')'); return yy::parser::make_R_PARENTHESIS(loc); }
"[" { TOKEN_CHAR('['); return yy::parser::make_L_BRACKET(loc); }
"]" { TOKEN_CHAR(']'); return yy::parser::make_R_BRACKET(loc); }

    /* Operator */
"+"   { TOKEN_CHAR('+'); return yy::parser::make_PLUS(loc); }
"-"   { TOKEN_CHAR('-'); return yy::parser::make_MINUS(loc); }
"*"   { TOKEN_CHAR('*'); return yy::parser::make_MULTIPLY(loc); }
"/"   { TOKEN_CHAR('/'); return yy::parser::make_DIVIDE(loc); }
"mod" { TOKEN(mod);      return yy::parser::make_MOD(loc); }
":="  { TOKEN(:=);       return yy::parser::make_ASSIGN(loc); }
"<"   { TOKEN_CHAR('<'); return yy::parser::make_LESS(loc); }
"<="  { TOKEN(<=);       return yy::parser::make_LESS_OR_EQUAL(loc); }
"<>"  { TOKEN(<>);       return yy::parser::make_NOT_EQUAL(loc); }
">="  { TOKEN(>=);       return yy::parser::make_GREATER_OR_EQUAL(loc); }
">"   { TOKEN_CHAR('>'); return yy::parser::make_GREATER(loc); }
"="   { TOKEN_CHAR('='); return yy::parser::make_EQUAL(loc); }
"and" { TOKEN(and);      return yy::parser::make_AND(loc); }
"or"  { TOKEN(or);       return yy::parser::make_OR(loc); }
"not" { TOKEN(not);      return yy::parser::make_NOT(loc); }

    /* Keyword */
"array"   { TOKEN(KWarray);   return yy::parser::make_ARRAY(loc); }
"begin"   { TOKEN(KWbegin);   return yy::parser::make_BEGIN_(loc); }
"boolean" { TOKEN(KWboolean); return yy::parser::make_BOOLEAN(loc); }
"def"     { TOKEN(KWdef);     return yy::parser::make_DEF(loc); }
"do"      { TOKEN(KWdo);      return yy::parser::make_DO(loc); }
"else"    { TOKEN(KWelse);    return yy::parser::make_ELSE(loc); }
"end"     { TOKEN(KWend);     return yy::parser::make_END(loc); }
"false"   { TOKEN(KWfalse);   return yy::parser::make_FALSE(loc); }
"for"     { TOKEN(KWfor);     return yy::parser::make_FOR(loc); }
"integer" { TOKEN(KWinteger); return yy::parser::make_INTEGER(loc); }
"if"      { TOKEN(KWif);      return yy::parser::make_IF(loc); }
"of"      { TOKEN(KWof);      return yy::parser::make_OF(loc); }
"print"   { TOKEN(KWprint);   return yy::parser::make_PRINT(loc); }
"read"    { TOKEN(KWread);    return yy::parser::make_READ(loc); }
"real"    { TOKEN(KWreal);    return yy::parser::make_REAL(loc); }
"string"  { TOKEN(KWstring);  return yy::parser::make_STRING(loc); }
"then"    { TOKEN(KWthen);    return yy::parser::make_THEN(loc); }
"to"      { TOKEN(KWto);      return yy::parser::make_TO(loc); }
"true"    { TOKEN(KWtrue);    return yy::parser::make_TRUE(loc); }
"return"  { TOKEN(KWreturn);  return yy::parser::make_RETURN(loc); }
"var"     { TOKEN(KWvar);     return yy::parser::make_VAR(loc); }
"while"   { TOKEN(KWwhile);   return yy::parser::make_WHILE(loc); }

    /* Identifier */
[a-zA-Z][a-zA-Z0-9]* {
    TOKEN_STRING(id, yytext);
    return yy::parser::make_ID(yytext, loc);
}

    /* Integer (decimal/octal) */
{integer} {
    TOKEN_STRING(integer, yytext);
    int64_t num = std::strtol(yytext, NULL, 10);
    return yy::parser::make_INT_LITERAL(num, loc);
}
0[0-7]+   {
    TOKEN_STRING(oct_integer, yytext);
    int64_t num = std::strtol(yytext, NULL, 8);
    return yy::parser::make_INT_LITERAL(num, loc);
}

    /* Floating-Point */
{float} {
    TOKEN_STRING(float, yytext);
    double num = std::strtod(yytext, NULL);
    return yy::parser::make_REAL_LITERAL(num, loc);
}

    /* Scientific Notation [Ee][+-]?[0-9]+ */
({integer}|{float})[Ee][+-]?({integer}) {
    TOKEN_STRING(scientific, yytext);
    double num = std::strtod(yytext, NULL);
    return yy::parser::make_REAL_LITERAL(num, loc);
}

    /* String */
\"([^"\n]|\"\")*\" {
    std::string string_literal(yytext + 1, strlen(yytext) - 2);
    size_t idx = 0;
    while (true) {
        idx = string_literal.find("\"\"", idx);
        if (idx == std::string::npos) break;
        string_literal.erase(idx, 2);
        string_literal.insert(idx, "\"");
    }
    TOKEN_STRING(string, string_literal.c_str());
    return yy::parser::make_STRING_LITERAL(string_literal, loc);
}
    /* Whitespace */
[ \t]+ {
    LIST;
    loc.step();
}

    /* Pseudocomment */
"//&"[STD][+-].* {
    LIST;
    char option = yytext[3];
    switch (option) {
    case 'S':
        drv.opt_src = (yytext[4] == '+') ? 1 : 0;
        break;
    case 'T':
        drv.opt_tok = (yytext[4] == '+') ? 1 : 0;
        break;
    case 'D':
        drv.opt_dump_symtab = (yytext[4] == '+') ? 1 : 0;
        break;
    }
}

    /* C++ Style Comment */
"//".* { LIST; }

    /* C Style Comment */
"/*"           { LIST; BEGIN(CCOMMENT); }
<CCOMMENT>"*/" { LIST; BEGIN(INITIAL); }
<CCOMMENT>.    { LIST; }

    /* Newline */
<INITIAL,CCOMMENT>\n {
    if (drv.opt_src) {
        std::cout << loc.begin.line << ": " << drv.buf << std::endl;
    }
    loc.lines();
    loc.step();
    drv.record_line_head(drv.buf.length() + 1);
    drv.clear_buf();
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", loc.begin.line, yytext);
    exit(-1);
}

<<EOF>>    { return yy::parser::make_EOF_(loc); };

%%

void driver::scan_begin() {
    if (!(yyin = fopen(file.c_str(), "r"))) {
        std::cerr << "Open file error" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void driver::record_line_head(long line_size) {
    line_head.push_back(line_size + *(line_head.end() - 1));
}

void driver::scan_list(std::string str) {
    buf += str;
}

void driver::clear_buf() {
    buf.clear();
}

void driver::scan_end() {
    fclose(yyin);
}

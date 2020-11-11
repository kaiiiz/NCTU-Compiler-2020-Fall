/* A Bison parser, made by GNU Bison 3.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DELIM_SCLN = 258,
    DELIM_CLN = 259,
    DELIM_COMMA = 260,
    DELIM_PTHS_L = 261,
    DELIM_PTHS_R = 262,
    DELIM_BRKT_L = 263,
    DELIM_BRKT_R = 264,
    OP_DIV = 265,
    OP_MUT = 266,
    OP_ADD = 267,
    OP_ASSIGN = 268,
    OP_MIN = 269,
    OP_L = 270,
    OP_LE = 271,
    OP_LG = 272,
    OP_GE = 273,
    OP_G = 274,
    OP_EQ = 275,
    OP_AND = 276,
    OP_OR = 277,
    OP_NOT = 278,
    KW_DEF = 279,
    KW_DO = 280,
    KW_FOR = 281,
    KW_TO = 282,
    KW_RET = 283,
    KW_WHILE = 284,
    KW_IF = 285,
    KW_THEN = 286,
    KW_ELSE = 287,
    KW_TRUE = 288,
    KW_FALSE = 289,
    KW_END = 290,
    KW_SCALAR_T = 291,
    KW_ARR = 292,
    KW_VAR = 293,
    KW_OF = 294,
    KW_BEGIN = 295,
    KW_PRINT = 296,
    KW_READ = 297,
    ID = 298,
    SCI = 299,
    FLOAT = 300,
    DEC_INT = 301,
    OCT_INT = 302,
    STR = 303
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

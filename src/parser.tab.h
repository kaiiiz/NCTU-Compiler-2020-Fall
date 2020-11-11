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
    OP_NEG = 265,
    OP_ASSIGN = 266,
    KW_DEF = 267,
    KW_DO = 268,
    KW_FOR = 269,
    KW_TO = 270,
    KW_RET = 271,
    KW_WHILE = 272,
    KW_IF = 273,
    KW_THEN = 274,
    KW_ELSE = 275,
    KW_TRUE = 276,
    KW_FALSE = 277,
    KW_END = 278,
    KW_SCALAR_T = 279,
    KW_ARR = 280,
    KW_VAR = 281,
    KW_OF = 282,
    KW_BEGIN = 283,
    KW_PRINT = 284,
    KW_READ = 285,
    ID = 286,
    SCI = 287,
    FLOAT = 288,
    DEC_INT = 289,
    OCT_INT = 290,
    STR = 291
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

/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

#ifndef YY_YY_SOURCE_HEADERS_PARSER_HPP_INCLUDED
# define YY_YY_SOURCE_HEADERS_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTEGER_CONSTANT = 258,
    FLOAT_CONSTANT = 259,
    NEGATION = 260,
    CF_IF = 261,
    CF_ELSE = 262,
    CF_FOR = 263,
    CF_WHILE = 264,
    CF_DO = 265,
    CF_RETURN = 266,
    LPARENT = 267,
    RPARENT = 268,
    LCURLYBRACE = 269,
    RCURLYBRACE = 270,
    SEMICOLON = 271,
    COMMA = 272,
    START_FUNC_DECL = 273,
    BOOLEAN_CONSTANT = 274,
    MINUS = 275,
    LINEAR_ARITHMETIC = 276,
    PLUS = 277,
    DATATYPE = 278,
    VOID = 279,
    IDENTIFIER = 280,
    CHARACTER_CONSTANT = 281,
    RELATIONAL_OPERATOR = 282,
    AND_OPERATOR = 283,
    OR_OPERATOR = 284,
    EQUALITY_OPERATOR = 285,
    ASSIGN = 286,
    MODIFIER = 287,
    DEC_FUNC_PREC = 288,
    DEC_FUNC_PREC2 = 289,
    UNARY_OPERATOR_PREC = 290
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 57 "./source/front-end/parser.y"

    int IntConst;
    float FloatConst;
    char CharConst;
    bool BoolConst;

    std::string *string;

    FuncDecl *funcDecl;
    FuncDeclParam *funcDeclParam;
    funcDeclParamList *funcDeclParamsList;
    funcDeclList *_funcDeclList;

    Expression *expr;
    Statement *stmnt;

    std::vector<Expression *> *exprList;
    std::vector<Statement *> *stmntList;
    Identifier *ident;

    ASTNode *astNode;

#line 116 "./source/headers/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SOURCE_HEADERS_PARSER_HPP_INCLUDED  */

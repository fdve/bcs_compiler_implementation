/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "./source/front-end/parser.y"

    #include <iostream>
    #include <vector>
    #include <algorithm>

    #include "../headers/ast.h"
    #include "../headers/astvisitors.h"
    #include "../headers/desugar.h"
    #include "../headers/util.h"
    #include "../headers/symboltable.h"
    #include "../headers/errorlogger.h"

    #define YYERROR_VERBOSE 1

    ASTNode *ASTRoot;

    extern FILE *yyin;
    extern int yylex();
    extern int yydebug;
    extern int yy_flex_debug;
    extern int yylineno;
    extern int yycolumn;


    /* Keep track of which function is currently being parsed, so that return
     * call nodes can be annotated with the name of which function they return to. */
    std::string currentFunc = "";

    /*-[] test yyyloc here*/
    void yyerror(std::string errorMsg) {
        Scope::ClearScopes();
        std::string currentFunc = "";
        ErrorLogger::parsingError = true;
        ErrorLogger::InsertMsg(ErrorLogger::ParserError(yylineno, errorMsg));

        cout_str("parser.y, in yyerror, testing yyloc");
        cout_str(std::to_string(yycolumn));


        // cout_str(std::to_string(@$.first_column));
    }

#line 113 "./source/front-end/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 224 "./source/front-end/parser.cpp"

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



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   356

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  133

#define YYUNDEFTOK  2
#define YYMAXUTOK   290


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   129,   129,   131,   140,   140,   155,   155,   173,   174,
     178,   178,   215,   216,   226,   227,   235,   236,   245,   246,
     267,   268,   269,   270,   271,   272,   276,   276,   287,   296,
     309,   322,   326,   330,   331,   335,   335,   335,   336,   336,
     337,   337,   337,   338,   338,   338,   342,   343,   348,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   373,
     377,   378,   382,   410,   411,   415,   416,   420,   421,   422,
     423
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER_CONSTANT", "FLOAT_CONSTANT",
  "NEGATION", "CF_IF", "CF_ELSE", "CF_FOR", "CF_WHILE", "CF_DO",
  "CF_RETURN", "LPARENT", "RPARENT", "LCURLYBRACE", "RCURLYBRACE",
  "SEMICOLON", "COMMA", "START_FUNC_DECL", "BOOLEAN_CONSTANT", "MINUS",
  "LINEAR_ARITHMETIC", "PLUS", "DATATYPE", "VOID", "IDENTIFIER",
  "CHARACTER_CONSTANT", "RELATIONAL_OPERATOR", "AND_OPERATOR",
  "OR_OPERATOR", "EQUALITY_OPERATOR", "ASSIGN", "MODIFIER",
  "DEC_FUNC_PREC", "DEC_FUNC_PREC2", "UNARY_OPERATOR_PREC", "$accept",
  "INPUT", "PROGRAM", "$@1", "START_FUNCTION", "$@2", "DEC_FUNCS",
  "DEC_FUNC", "$@3", "DEC_FUNC_CALL_PARAMS", "DEC_FUNC_CALL_PARAMS_OPT",
  "RETURNTYPE", "STATEMENTS", "STATEMENT", "FUNCTION_BLOCK", "$@4",
  "VARIABLE_DEF", "ASSIGNMENT", "EXPRESSION_SC", "RETURN_CALL",
  "RETURN_CALL_OPT", "CONTROL_FLOW", "$@5", "$@6", "$@7", "$@8", "$@9",
  "$@10", "$@11", "CONTROL_FLOW_OPT_ELSE", "EXPRESSION", "UNARY_OPERATOR",
  "FUNCTION_CALL", "FUNC_CALL_PARAM", "FUNC_CALL_PARAM_OPT", "CONSTANT", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290
};
# endif

#define YYPACT_NINF (-76)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-45)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -76,     1,   -76,   -76,   -76,    26,   -76,   -76,   -76,   -76,
     -76,   -19,     3,    42,   -76,     0,    12,   -76,    42,   -76,
      18,    13,    20,   -76,   -76,    31,    52,   -76,    42,   -76,
     -76,   -76,   -76,   196,   -76,   196,    40,   196,   196,   -76,
     -76,   -76,    34,    -4,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,   218,   196,   -76,   -76,    48,    39,   -11,    51,
     203,   -76,    60,   311,   183,    58,   196,   196,   196,   -76,
     196,   196,   196,   196,   196,   196,   196,   -76,    31,   -76,
     196,   -76,   -76,   -76,   -76,   196,   307,    70,   233,   248,
      77,   -76,    77,    72,   326,   322,     9,   -76,   -76,   263,
     -76,    76,   278,   196,   -76,   -76,   -76,   -76,   100,    75,
     124,    82,   -76,   307,    92,    37,    99,   101,   105,   -76,
     110,   -76,   -76,   196,   104,   -76,   148,   293,   -76,   -76,
     -76,   172,   -76
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     4,     1,     3,     8,     0,     6,    16,    17,     8,
       9,     0,     0,     5,    10,     0,     0,    26,    12,     7,
       0,     0,     0,    18,    26,    14,    26,    11,     0,    13,
      67,    68,    61,     0,    38,     0,     0,    33,     0,    27,
      69,    60,     0,    58,    70,    19,    25,    20,    21,    22,
      23,    24,     0,     0,    57,    59,     0,    58,     0,     0,
       0,    43,     0,    34,     0,     0,    63,     0,     0,    31,
       0,     0,     0,     0,     0,     0,     0,    48,    14,    35,
       0,    40,    18,    32,    56,     0,    65,     0,     0,     0,
      51,    49,    50,    52,    53,    54,    55,    15,    18,     0,
      18,    26,     0,     0,    64,    62,    29,    30,    26,     0,
      26,     0,    28,    65,     0,     0,     0,     0,     0,    66,
      46,    18,    42,     0,     0,    37,    26,     0,    18,    39,
      45,    26,    47
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -76,   -76,   -76,   -76,   -76,   -76,   112,   -76,   -76,   -76,
      44,    -6,   -75,   -76,    -3,   -76,    65,    22,   -76,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -33,   -76,   -76,   -76,    24,   -76
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     3,     4,     9,    12,     5,    10,    16,    21,
      29,    11,    26,    45,    46,    20,    47,    48,    49,    50,
      62,    51,    98,   114,    59,   100,   117,    82,   111,   125,
      52,    53,    54,    87,   104,    55
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,     2,    60,    79,    63,    64,    14,   101,    66,    70,
      71,    72,    22,    17,    19,    15,    73,    74,    75,    76,
      77,    27,    56,   108,    18,   110,    24,    67,    68,    70,
      71,    72,    23,    86,    88,    89,    73,    90,    91,    92,
      93,    94,    95,    96,     6,    25,   126,    99,    28,     7,
       8,    66,   102,   131,    61,    30,    31,    32,    33,    65,
      34,    35,    36,    37,    38,     7,     8,    39,    67,    68,
     113,    40,    41,    78,    42,    42,    83,    43,    44,    30,
      31,    32,    33,   105,    34,    35,    36,    37,    38,    85,
     127,   -44,    70,    71,    72,    40,    41,   118,    71,    42,
     115,    43,    44,    30,    31,    32,    33,   120,    34,    35,
      36,    37,    38,   121,   123,   -36,   122,   124,   128,    40,
      41,    13,    97,    42,    80,    43,    44,    30,    31,    32,
      33,   116,    34,    35,    36,    37,    38,   119,     0,   -41,
       0,     0,     0,    40,    41,     0,     0,    42,     0,    43,
      44,    30,    31,    32,    33,     0,    34,    35,    36,    37,
      38,     0,     0,   129,     0,     0,     0,    40,    41,     0,
       0,    42,     0,    43,    44,    30,    31,    32,    33,     0,
      34,    35,    36,    37,    38,     0,     0,   132,     0,     0,
       0,    40,    41,     0,     0,    42,    84,    43,    44,    30,
      31,    32,     0,    70,    71,    72,     0,     0,    38,     0,
      73,    74,    75,    76,     0,    40,    41,    81,     0,     0,
       0,    57,    44,    70,    71,    72,     0,     0,     0,     0,
      73,    74,    75,    76,    69,     0,     0,     0,    70,    71,
      72,     0,     0,     0,     0,    73,    74,    75,    76,   106,
       0,     0,     0,    70,    71,    72,     0,     0,     0,     0,
      73,    74,    75,    76,   107,     0,     0,     0,    70,    71,
      72,     0,     0,     0,     0,    73,    74,    75,    76,   109,
       0,     0,     0,    70,    71,    72,     0,     0,     0,     0,
      73,    74,    75,    76,   112,     0,     0,     0,    70,    71,
      72,     0,     0,     0,     0,    73,    74,    75,    76,   130,
       0,     0,     0,    70,    71,    72,     0,     0,     0,     0,
      73,    74,    75,    76,   103,     0,     0,    70,    71,    72,
       0,    70,    71,    72,    73,    74,    75,    76,    73,    74,
      75,    76,    70,    71,    72,     0,    70,    71,    72,    73,
      74,     0,    76,    73,     0,     0,    76
};

static const yytype_int16 yycheck[] =
{
      33,     0,    35,    14,    37,    38,    25,    82,    12,    20,
      21,    22,    18,    13,    17,    12,    27,    28,    29,    30,
      53,    24,    28,    98,    12,   100,    13,    31,    32,    20,
      21,    22,    14,    66,    67,    68,    27,    70,    71,    72,
      73,    74,    75,    76,    18,    25,   121,    80,    17,    23,
      24,    12,    85,   128,    14,     3,     4,     5,     6,    25,
       8,     9,    10,    11,    12,    23,    24,    15,    31,    32,
     103,    19,    20,    25,    23,    23,    16,    25,    26,     3,
       4,     5,     6,    13,     8,     9,    10,    11,    12,    31,
     123,    15,    20,    21,    22,    19,    20,    15,    21,    23,
      25,    25,    26,     3,     4,     5,     6,    15,     8,     9,
      10,    11,    12,    14,     9,    15,    15,     7,    14,    19,
      20,     9,    78,    23,    59,    25,    26,     3,     4,     5,
       6,   109,     8,     9,    10,    11,    12,   113,    -1,    15,
      -1,    -1,    -1,    19,    20,    -1,    -1,    23,    -1,    25,
      26,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    -1,    -1,    15,    -1,    -1,    -1,    19,    20,    -1,
      -1,    23,    -1,    25,    26,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      -1,    19,    20,    -1,    -1,    23,    13,    25,    26,     3,
       4,     5,    -1,    20,    21,    22,    -1,    -1,    12,    -1,
      27,    28,    29,    30,    -1,    19,    20,    14,    -1,    -1,
      -1,    25,    26,    20,    21,    22,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    16,    -1,    -1,    -1,    20,    21,
      22,    -1,    -1,    -1,    -1,    27,    28,    29,    30,    16,
      -1,    -1,    -1,    20,    21,    22,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    16,    -1,    -1,    -1,    20,    21,
      22,    -1,    -1,    -1,    -1,    27,    28,    29,    30,    16,
      -1,    -1,    -1,    20,    21,    22,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    16,    -1,    -1,    -1,    20,    21,
      22,    -1,    -1,    -1,    -1,    27,    28,    29,    30,    16,
      -1,    -1,    -1,    20,    21,    22,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    17,    -1,    -1,    20,    21,    22,
      -1,    20,    21,    22,    27,    28,    29,    30,    27,    28,
      29,    30,    20,    21,    22,    -1,    20,    21,    22,    27,
      28,    -1,    30,    27,    -1,    -1,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    37,     0,    38,    39,    42,    18,    23,    24,    40,
      43,    47,    41,    42,    25,    12,    44,    13,    12,    50,
      51,    45,    47,    14,    13,    25,    48,    50,    17,    46,
       3,     4,     5,     6,     8,     9,    10,    11,    12,    15,
      19,    20,    23,    25,    26,    49,    50,    52,    53,    54,
      55,    57,    66,    67,    68,    71,    47,    25,    66,    60,
      66,    14,    56,    66,    66,    25,    12,    31,    32,    16,
      20,    21,    22,    27,    28,    29,    30,    66,    25,    14,
      52,    14,    63,    16,    13,    31,    66,    69,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    46,    58,    66,
      61,    48,    66,    17,    70,    13,    16,    16,    48,    16,
      48,    64,    16,    66,    59,    25,    53,    62,    15,    70,
      15,    14,    15,     9,     7,    65,    48,    66,    14,    15,
      16,    48,    15
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    37,    39,    38,    41,    40,    42,    42,
      44,    43,    45,    45,    46,    46,    47,    47,    48,    48,
      49,    49,    49,    49,    49,    49,    51,    50,    52,    53,
      53,    54,    55,    56,    56,    58,    59,    57,    60,    57,
      61,    62,    57,    63,    64,    57,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      67,    67,    68,    69,    69,    70,    70,    71,    71,    71,
      71
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     5,     0,     2,
       0,     7,     0,     3,     0,     4,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     1,     0,     4,     5,     4,
       4,     2,     3,     0,     1,     0,     0,     8,     0,     9,
       0,     0,     7,     0,     0,     9,     0,     4,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     4,     0,     2,     0,     3,     1,     1,     1,
       1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4:
#line 140 "./source/front-end/parser.y"
    {
        Scope::PushScope();
    }
#line 1656 "./source/front-end/parser.cpp"
    break;

  case 5:
#line 144 "./source/front-end/parser.y"
    {
        ASTRoot = new Program((yyvsp[-2]._funcDeclList), (yyvsp[-1].funcDecl), (yyvsp[0]._funcDeclList));
        Scope::PopScope();
    }
#line 1665 "./source/front-end/parser.cpp"
    break;

  case 6:
#line 155 "./source/front-end/parser.y"
                    {
                                        Scope::AddSymbol("int", "start", &(yylsp[0]));
                                        currentFunc = "start";

                    }
#line 1675 "./source/front-end/parser.cpp"
    break;

  case 7:
#line 163 "./source/front-end/parser.y"
    {
        Identifier *idNode = new Identifier("int", "start", Scope::GetSymbol("start"));
        (yyval.funcDecl) = new FuncDecl("int", idNode, nullptr, dynamic_cast<FunctionBlock *>((yyvsp[0].stmnt))->stmnts, &(yylsp[-4]));


    }
#line 1686 "./source/front-end/parser.cpp"
    break;

  case 8:
#line 173 "./source/front-end/parser.y"
                                                        { (yyval._funcDeclList) = new funcDeclList(); }
#line 1692 "./source/front-end/parser.cpp"
    break;

  case 9:
#line 174 "./source/front-end/parser.y"
                                                                            { (yyval._funcDeclList)->push_back((yyvsp[0].funcDecl)); }
#line 1698 "./source/front-end/parser.cpp"
    break;

  case 10:
#line 178 "./source/front-end/parser.y"
                           {
                                    Scope::AddSymbol(*(yyvsp[-1].string), *(yyvsp[0].string), &(yylsp[0]));
                                    Scope::PushScope();
                                    currentFunc = *(yyvsp[0].string);


                                    // if identifier was a node instead, at this point we could enter the returned ref
                                    // from the add to scope func call?
                                    // or just do getSymbol below? :)
                            }
#line 1713 "./source/front-end/parser.cpp"
    break;

  case 11:
#line 193 "./source/front-end/parser.y"
    {
        // // cout_str("PRINTING SCOPES AFTER PARSING FUNCTION");
        // Scope::PrintScopes(Scope::globalScope, 0);



        Identifier *idNode = new Identifier(*(yyvsp[-6].string), *(yyvsp[-5].string), Scope::GetSymbol(*(yyvsp[-5].string)));
        (yyval.funcDecl) = new FuncDecl(*(yyvsp[-6].string), idNode, (yyvsp[-2].funcDeclParamsList), dynamic_cast<FunctionBlock *>((yyvsp[0].stmnt))->stmnts, &(yylsp[-6]));

        Scope::GetSymbol(*(yyvsp[-5].string))->params = (yyvsp[-2].funcDeclParamsList);

        // symTableEntry *entry = Scope::GetSymbol(*$2);
        // SymbolTable::PrintEntry(*entry, 123);




        Scope::PopScope();
    }
#line 1737 "./source/front-end/parser.cpp"
    break;

  case 12:
#line 215 "./source/front-end/parser.y"
                                                        { (yyval.funcDeclParamsList) = nullptr;  }
#line 1743 "./source/front-end/parser.cpp"
    break;

  case 13:
#line 217 "./source/front-end/parser.y"
    {
        (yyvsp[0].funcDeclParamsList)->push_back(new FuncDeclParam(*(yyvsp[-2].string), new Identifier(*(yyvsp[-2].string), *(yyvsp[-1].string), Scope::AddSymbol(*(yyvsp[-2].string), *(yyvsp[-1].string), &(yylsp[-1])))));
        (yyval.funcDeclParamsList) = (yyvsp[0].funcDeclParamsList);
        std::reverse((yyval.funcDeclParamsList)->begin(), (yyval.funcDeclParamsList)->end());
        ;
    }
#line 1754 "./source/front-end/parser.cpp"
    break;

  case 14:
#line 226 "./source/front-end/parser.y"
                                                        { (yyval.funcDeclParamsList) = new funcDeclParamList(); }
#line 1760 "./source/front-end/parser.cpp"
    break;

  case 15:
#line 228 "./source/front-end/parser.y"
    {
        (yyvsp[0].funcDeclParamsList)->push_back(new FuncDeclParam(*(yyvsp[-2].string), new Identifier(*(yyvsp[-2].string), *(yyvsp[-1].string), Scope::AddSymbol(*(yyvsp[-2].string), *(yyvsp[-1].string), &(yylsp[-1])))));
        (yyval.funcDeclParamsList) = (yyvsp[0].funcDeclParamsList);
    }
#line 1769 "./source/front-end/parser.cpp"
    break;

  case 16:
#line 235 "./source/front-end/parser.y"
                                                        { (yyval.string) = (yyvsp[0].string); }
#line 1775 "./source/front-end/parser.cpp"
    break;

  case 17:
#line 236 "./source/front-end/parser.y"
                                                        { (yyval.string) = (yyvsp[0].string); }
#line 1781 "./source/front-end/parser.cpp"
    break;

  case 18:
#line 245 "./source/front-end/parser.y"
                                                        { (yyval.stmntList) = new stmntList(); }
#line 1787 "./source/front-end/parser.cpp"
    break;

  case 19:
#line 247 "./source/front-end/parser.y"
    {
        if(CFDoWhile *doWhile = dynamic_cast<CFDoWhile*>((yyvsp[0].stmnt))) {
            (yyvsp[0].stmnt) = desugarDoWhile((yyval.stmntList), doWhile);
        }
        else if(CFFor *forNode = dynamic_cast<CFFor*>((yyvsp[0].stmnt))) {
            (yyvsp[0].stmnt) = desugarFor((yyval.stmntList), forNode);
        }

        if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>((yyvsp[0].stmnt))) {
            for(Statement *stmnt : *funcBlock->stmnts) {
                (yyval.stmntList)->push_back(stmnt);
            }
        }
        else {
            (yyval.stmntList)->push_back((yyvsp[0].stmnt));
        }
    }
#line 1809 "./source/front-end/parser.cpp"
    break;

  case 20:
#line 267 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = (yyvsp[0].stmnt); }
#line 1815 "./source/front-end/parser.cpp"
    break;

  case 21:
#line 268 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = (yyvsp[0].stmnt); }
#line 1821 "./source/front-end/parser.cpp"
    break;

  case 22:
#line 269 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = (yyvsp[0].stmnt); }
#line 1827 "./source/front-end/parser.cpp"
    break;

  case 23:
#line 270 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = (yyvsp[0].stmnt); }
#line 1833 "./source/front-end/parser.cpp"
    break;

  case 24:
#line 271 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = (yyvsp[0].stmnt); }
#line 1839 "./source/front-end/parser.cpp"
    break;

  case 25:
#line 272 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = (yyvsp[0].stmnt); }
#line 1845 "./source/front-end/parser.cpp"
    break;

  case 26:
#line 276 "./source/front-end/parser.y"
    {
        Scope::PushScope();
    }
#line 1853 "./source/front-end/parser.cpp"
    break;

  case 27:
#line 280 "./source/front-end/parser.y"
    {
        (yyval.stmnt) = new FunctionBlock((yyvsp[-1].stmntList));
        Scope::PopScope();
    }
#line 1862 "./source/front-end/parser.cpp"
    break;

  case 28:
#line 288 "./source/front-end/parser.y"
    {
        (yyval.stmnt) = new VariableDef(*(yyvsp[-4].string), new Identifier(*(yyvsp[-4].string), *(yyvsp[-3].string), Scope::AddSymbol(*(yyvsp[-4].string), *(yyvsp[-3].string), &(yylsp[-3]))), (yyvsp[-1].expr), &(yylsp[-3]));
        // cout_str("VAR DEF CHECKING column " + *$2);
        // cout_str(std::to_string(@2.first_column));
    }
#line 1872 "./source/front-end/parser.cpp"
    break;

  case 29:
#line 297 "./source/front-end/parser.y"
      {
          if(Scope::ContainsKey(*(yyvsp[-3].string))) {
              (yyval.stmnt) = new Assignment(new Identifier("", *(yyvsp[-3].string), Scope::GetSymbol(*(yyvsp[-3].string))), "=", (yyvsp[-1].expr), &(yylsp[-3]));
          }
          else {

              // Scope::PrintScopes(Scope::globalScope, 0);
              (yyval.stmnt) = new Assignment(new Identifier(*(yyvsp[-3].string)), "=", (yyvsp[-1].expr), &(yylsp[-3]));
              ErrorLogger::InsertMsg(ErrorLogger::UndefinedVar(newInfo(&(yylsp[-3])), *(yyvsp[-3].string)));
          }
      }
#line 1888 "./source/front-end/parser.cpp"
    break;

  case 30:
#line 310 "./source/front-end/parser.y"
    {
        if(Scope::ContainsKey(*(yyvsp[-3].string))) {
            (yyval.stmnt) = new Assignment(new Identifier("", *(yyvsp[-3].string), Scope::GetSymbol(*(yyvsp[-3].string))), *(yyvsp[-2].string), (yyvsp[-1].expr), &(yylsp[-3]));
        }
        else {
            (yyval.stmnt) = new Assignment(new Identifier(*(yyvsp[-3].string)), *(yyvsp[-2].string), (yyvsp[-1].expr), &(yylsp[-3]));
            ErrorLogger::InsertMsg(ErrorLogger::UndefinedVar(newInfo(&(yylsp[-3])), *(yyvsp[-3].string)));
        }
    }
#line 1902 "./source/front-end/parser.cpp"
    break;

  case 31:
#line 322 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = new ExprStmnt((yyvsp[-1].expr)); }
#line 1908 "./source/front-end/parser.cpp"
    break;

  case 32:
#line 326 "./source/front-end/parser.y"
                                                      { (yyval.stmnt) = new ReturnCall((yyvsp[-1].expr), new Identifier("", currentFunc, Scope::GetSymbol(currentFunc)), &(yylsp[-1])); }
#line 1914 "./source/front-end/parser.cpp"
    break;

  case 33:
#line 330 "./source/front-end/parser.y"
                                                      { (yyval.expr) = nullptr; }
#line 1920 "./source/front-end/parser.cpp"
    break;

  case 34:
#line 331 "./source/front-end/parser.y"
                                                      { (yyval.expr) = (yyvsp[0].expr); }
#line 1926 "./source/front-end/parser.cpp"
    break;

  case 35:
#line 335 "./source/front-end/parser.y"
                                   { Scope::PushScope(); }
#line 1932 "./source/front-end/parser.cpp"
    break;

  case 36:
#line 335 "./source/front-end/parser.y"
                                                                      { Scope::PopScope(); }
#line 1938 "./source/front-end/parser.cpp"
    break;

  case 37:
#line 335 "./source/front-end/parser.y"
                                                                                                                                { (yyval.stmnt) = new CFIf((yyvsp[-6].expr), (yyvsp[-3].stmntList), (yyvsp[0].stmntList), &(yylsp[-6])); }
#line 1944 "./source/front-end/parser.cpp"
    break;

  case 38:
#line 336 "./source/front-end/parser.y"
             { Scope::PushScope(); }
#line 1950 "./source/front-end/parser.cpp"
    break;

  case 39:
#line 336 "./source/front-end/parser.y"
                                                                                                                                { Scope::PopScope(); (yyval.stmnt) = new CFFor((yyvsp[-6].stmnt), (yyvsp[-5].expr), (yyvsp[-3].stmnt), (yyvsp[-1].stmntList), &(yylsp[-5])); }
#line 1956 "./source/front-end/parser.cpp"
    break;

  case 40:
#line 337 "./source/front-end/parser.y"
                                      { Scope::PushScope(); }
#line 1962 "./source/front-end/parser.cpp"
    break;

  case 41:
#line 337 "./source/front-end/parser.y"
                                                                         { Scope::PopScope(); }
#line 1968 "./source/front-end/parser.cpp"
    break;

  case 42:
#line 337 "./source/front-end/parser.y"
                                                                                                                                { (yyval.stmnt) = new CFWhile((yyvsp[-5].expr), (yyvsp[-2].stmntList), &(yylsp[-5])); }
#line 1974 "./source/front-end/parser.cpp"
    break;

  case 43:
#line 338 "./source/front-end/parser.y"
                         { Scope::PushScope(); }
#line 1980 "./source/front-end/parser.cpp"
    break;

  case 44:
#line 338 "./source/front-end/parser.y"
                                                            { Scope::PopScope(); }
#line 1986 "./source/front-end/parser.cpp"
    break;

  case 45:
#line 338 "./source/front-end/parser.y"
                                                                                                                                { (yyval.stmnt) = new CFDoWhile((yyvsp[-1].expr), (yyvsp[-5].stmntList), &(yylsp[-1])); }
#line 1992 "./source/front-end/parser.cpp"
    break;

  case 46:
#line 342 "./source/front-end/parser.y"
                                                     { (yyval.stmntList) = nullptr; }
#line 1998 "./source/front-end/parser.cpp"
    break;

  case 47:
#line 343 "./source/front-end/parser.y"
                                                     { (yyval.stmntList) = (yyvsp[-1].stmntList); }
#line 2004 "./source/front-end/parser.cpp"
    break;

  case 48:
#line 349 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new UnaryOpExpr((yyvsp[-1].CharConst), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2010 "./source/front-end/parser.cpp"
    break;

  case 49:
#line 350 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), *(yyvsp[-1].string), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2016 "./source/front-end/parser.cpp"
    break;

  case 50:
#line 351 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), *(yyvsp[-1].string), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2022 "./source/front-end/parser.cpp"
    break;

  case 51:
#line 352 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), std::string("-"), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2028 "./source/front-end/parser.cpp"
    break;

  case 52:
#line 353 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), *(yyvsp[-1].string), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2034 "./source/front-end/parser.cpp"
    break;

  case 53:
#line 354 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), *(yyvsp[-1].string), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2040 "./source/front-end/parser.cpp"
    break;

  case 54:
#line 355 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), *(yyvsp[-1].string), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2046 "./source/front-end/parser.cpp"
    break;

  case 55:
#line 356 "./source/front-end/parser.y"
                                                      { (yyval.expr) = new BitwiseOpExpr((yyvsp[-2].expr), *(yyvsp[-1].string), (yyvsp[0].expr), &(yylsp[-1])); }
#line 2052 "./source/front-end/parser.cpp"
    break;

  case 56:
#line 357 "./source/front-end/parser.y"
                                                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 2058 "./source/front-end/parser.cpp"
    break;

  case 57:
#line 358 "./source/front-end/parser.y"
                                                      { (yyval.expr) = (yyvsp[0].expr); }
#line 2064 "./source/front-end/parser.cpp"
    break;

  case 58:
#line 360 "./source/front-end/parser.y"
    {
        if(Scope::ContainsKey(*(yyvsp[0].string))) {
            (yyval.expr) = new Identifier(Scope::GetSymbol(*(yyvsp[0].string)));

            /* -[] everything above seems to work. Why does the ref encoutnerd in print visitor is diffierent?
              Something to do with pushing / popping scopes? */
        }
        else {
            	/* -[] shouldnt this give an error? unknown ID? <- think the scope contains call already does that*/
            (yyval.expr) = new Identifier(*(yyvsp[0].string), &(yylsp[0]));
        }

    }
#line 2082 "./source/front-end/parser.cpp"
    break;

  case 59:
#line 373 "./source/front-end/parser.y"
                                                    { (yyval.expr) = (yyvsp[0].expr); }
#line 2088 "./source/front-end/parser.cpp"
    break;

  case 60:
#line 377 "./source/front-end/parser.y"
                                                    { (yyval.CharConst) = (yyvsp[0].CharConst); }
#line 2094 "./source/front-end/parser.cpp"
    break;

  case 61:
#line 378 "./source/front-end/parser.y"
                                                    { (yyval.CharConst) = (yyvsp[0].CharConst); }
#line 2100 "./source/front-end/parser.cpp"
    break;

  case 62:
#line 383 "./source/front-end/parser.y"
    {
        if((yyvsp[-1].exprList) != nullptr) {
            std::reverse((yyvsp[-1].exprList)->begin(), (yyvsp[-1].exprList)->end());
        }
        // we add the type of the function call by looking up
        // the ID in the existing scopes

        if(Scope::ContainsKey(*(yyvsp[-3].string))) {
            // cout_str("EXPR : FUNCTION_CALL in parser.y");
            // cout_str("EXPR : FUNCTION_CALL checkign if entry has params set : ");
            // cout_str("EXPR : FUNCTION_CALL identifier : " + *$1);

            symTableEntry *entry = Scope::GetSymbol(*(yyvsp[-3].string));

            if(entry->params == nullptr) {
                // cout_str("EXPR : FUNCTION_CALL entry->params == nullptr");
            }

            (yyval.expr) = new FuncCall(entry->type, new Identifier(entry), (yyvsp[-1].exprList), &(yylsp[-3]));
        }
        else {
            (yyval.expr) = new FuncCall("unknown_return_type", new Identifier(*(yyvsp[-3].string)), (yyvsp[-1].exprList), &(yylsp[-3]));
        }
    }
#line 2129 "./source/front-end/parser.cpp"
    break;

  case 63:
#line 410 "./source/front-end/parser.y"
                                                    { (yyval.exprList) = nullptr; }
#line 2135 "./source/front-end/parser.cpp"
    break;

  case 64:
#line 411 "./source/front-end/parser.y"
                                                    { (yyvsp[0].exprList)->push_back((yyvsp[-1].expr)); (yyval.exprList) = (yyvsp[0].exprList); }
#line 2141 "./source/front-end/parser.cpp"
    break;

  case 65:
#line 415 "./source/front-end/parser.y"
                                                    { (yyval.exprList) = new exprList(); }
#line 2147 "./source/front-end/parser.cpp"
    break;

  case 66:
#line 416 "./source/front-end/parser.y"
                                                    { (yyvsp[0].exprList)->push_back((yyvsp[-1].expr)); (yyval.exprList) = (yyvsp[0].exprList); }
#line 2153 "./source/front-end/parser.cpp"
    break;

  case 67:
#line 420 "./source/front-end/parser.y"
                                                    { (yyval.expr) = new IntConst((yyvsp[0].IntConst)); }
#line 2159 "./source/front-end/parser.cpp"
    break;

  case 68:
#line 421 "./source/front-end/parser.y"
                                                    { (yyval.expr) = new FloatConst((yyvsp[0].FloatConst)); }
#line 2165 "./source/front-end/parser.cpp"
    break;

  case 69:
#line 422 "./source/front-end/parser.y"
                                                    { (yyval.expr) = new BoolConst((yyvsp[0].BoolConst)); }
#line 2171 "./source/front-end/parser.cpp"
    break;

  case 70:
#line 423 "./source/front-end/parser.y"
                                                    { (yyval.expr) = new CharConst(*(yyvsp[0].string)); }
#line 2177 "./source/front-end/parser.cpp"
    break;


#line 2181 "./source/front-end/parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 426 "./source/front-end/parser.y"

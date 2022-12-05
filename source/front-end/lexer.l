/* Flex file where input data is matched against declared regular expressions.
 * Return matched tokens, or do nothing if the input data matches no token pattern. */

%{
    #include <iostream>
    #include <sstream>
    #include <string.h>
    #include "../headers/ast.h"
    #include "../headers/parser.hpp"

    #define ALLOC_STRING yylval.string = new std::string(yytext, yyleng);

    int yycolumn = 1;
    int yyloc_first_col_cpy = 1;

    /* Store the source file location of tokens. */
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
                           yylloc.first_column = yycolumn; \
                           yyloc_first_col_cpy = yycolumn; \
                           yylloc.last_column = yycolumn + yyleng - 1; \
                           yycolumn += yyleng;

    /* Using strtol() instead of atoi() since atoi is not safe. */
    #define INT yylval.IntConst = strtol(yytext, NULL, 10);

    bool is_true_const(const char *str) {
        return str[0] == 't';
    }

    /* Flush buffer and reset yylineo. */
    void InitFlex() {
        yylineno = 1;
        YY_FLUSH_BUFFER;
    }
%}

/* Compilation errors occur if you compile flex and bison files together, and in
 * the corresponding bison file you do not explicitly declare tokens you return here. */

%option noyywrap yylineno nounput

START_FUNC_DECL     "int"[ ]+"start"
DATATYPE            bool|int|char|float
VOID                void
CF_IF               "If"
CF_ELSE             "Else"
CF_FOR              "For"
CF_WHILE            "While"
CF_DO               "Do"
CF_RETURN           "Return"
NEGATION            "!"
LINEAR_ARITHMETIC   "*"|"/"|"%"
PLUS                "+"
MINUS               "-"
RELATIONAL_OPERATOR "<"|">"|"<="|">="
AND_OPERATOR        "&&"
OR_OPERATOR         "||"
EQUALITY_OPERATOR   "=="|"!="
MODIFIER            "+="|"-="|"*="|"/="|"%="
INTEGER_CONSTANT    [0-9]+
FLOAT_CONSTANT      ([0-9]+"."[0-9]+)|("."[0-9]+)
BOOLEAN_CONSTANT    "true"|"false"
CHARACTER_CONSTANT  '(\\.|[^\\'])+'
IDENTIFIER          [a-zA-Z]([a-zA-Z0-9_])*
LPARENT             "("
RPARENT             ")"
LCURLYBRACE         "{"
RCURLYBRACE         "}"
SEMICOLON           ";"
COMMA               ","
ASSIGN              "="

%%

{MODIFIER}                          { ALLOC_STRING; return(MODIFIER); }
{VOID}                              { ALLOC_STRING; return(VOID); }
{START_FUNC_DECL}                   { return(START_FUNC_DECL); }
{DATATYPE}                          { ALLOC_STRING; return(DATATYPE); }
{CF_IF}                             { return(CF_IF); }
{CF_ELSE}                           { return(CF_ELSE); }
{CF_FOR}                            { return(CF_FOR); }
{CF_WHILE}                          { return(CF_WHILE); }
{CF_DO}                             { return(CF_DO); }
{CF_RETURN}                         { return(CF_RETURN); }
{LINEAR_ARITHMETIC}                 { ALLOC_STRING; return(LINEAR_ARITHMETIC); }
{PLUS}                              { ALLOC_STRING; return(PLUS); }
{RELATIONAL_OPERATOR}               { ALLOC_STRING; return(RELATIONAL_OPERATOR);}
{AND_OPERATOR}                      { ALLOC_STRING; return(AND_OPERATOR);  }
{OR_OPERATOR}                       { ALLOC_STRING; return(OR_OPERATOR);  }
{EQUALITY_OPERATOR}                 { ALLOC_STRING; return(EQUALITY_OPERATOR);   }
{MINUS}                             { yylval.CharConst = *yytext; return(MINUS); }
{NEGATION}                          { yylval.CharConst = *yytext; return(NEGATION); }
{INTEGER_CONSTANT}                  { INT; return(INTEGER_CONSTANT); }
{FLOAT_CONSTANT}                    { yylval.FloatConst = strtof(yytext, NULL); return(FLOAT_CONSTANT); }
{BOOLEAN_CONSTANT}                  { yylval.BoolConst = is_true_const(yytext); return(BOOLEAN_CONSTANT); }
{CHARACTER_CONSTANT}                { ALLOC_STRING; return(CHARACTER_CONSTANT);}
{ASSIGN}                            { return(ASSIGN); }
{IDENTIFIER}                        { ALLOC_STRING; return(IDENTIFIER); }
{LPARENT}                           { return(LPARENT);  }
{RPARENT}                           { return(RPARENT);  }
{LCURLYBRACE}                       { return(LCURLYBRACE);  }
{RCURLYBRACE}                       { return(RCURLYBRACE);  }
{SEMICOLON}                         { return(SEMICOLON); }
{COMMA}                             { return(COMMA); }
" "                                 { /* Eat up whitespace and do nothing. */}
"//".*                              { /* Eat up single line comments and do nothing. */ }
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] { /* Eat up (possibly) multiple line comments and do nothing. */ }
\n                                  { yycolumn = 1; /* Resetting the current column counter. */ }
.                                   { /* Ignore every input which does not match any of the above inputs. */ }

%%

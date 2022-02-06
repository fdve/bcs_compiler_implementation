%{
    #include <iostream>
    #include <sstream>
    using namespace std;

    extern "C" {
        int yylex(void);
    }

    #include "parser.tab.h"
%}



%option noyywrap

TYPE                bool|int|char|float|string
CONDITIONAL         If|For|While|Return|Continue
UNARY_OPERATOR      "!"|"-"
ARITHMETIC_OPERATOR "+"|"-"|"*"|"%"
RELATIONAL_OPERATOR "<"|">"|"<="|">="
EQUALITY_OPERATOR   "=="|"!="
LOGICAL_OPERATOR    "&&"|"||"
FLOAT_CONSTANT      ([0-9]+"."[0-9]+)|("."[0-9]+)
INTEGER_CONSTANT    [0-9]+
BOOLEAN_CONSTANT    true|false
CHARACTER_CONSTANT  ("'"[0-9a-zA-Z]"'")
IDENTIFIER          [a-zA-Z]([a-zA-Z0-9])*


%%
{TYPE}                              { cout << "TYPE <" << yytext << ">" << endl; }
{CONDITIONAL}                       { cout << "CONDITIONAL <" << yytext << ">" << endl; }
{UNARY_OPERATOR}                    { cout << "UNARY_OPERATOR <" << yytext << ">" << endl; }
{ARITHMETIC_OPERATOR}               { cout << "ARITHMETIC_OPERATOR <" << yytext << ">" << endl; }
{RELATIONAL_OPERATOR}               { cout << "RELATIONAL_OPERATOR <" << yytext << ">" << endl; }
{EQUALITY_OPERATOR}                 { cout << "EQUALITY_OPERATOR <" << yytext << ">" << endl; }
{LOGICAL_OPERATOR}                  { cout << "LOGICAL_OPERATOR <" << yytext << ">" << endl; }
{INTEGER_CONSTANT}                  { cout << "INTEGER_CONSTANT <" << yytext << ">" << endl; }
{FLOAT_CONSTANT}                    { cout << "FLOAT_CONSTANT <" << yytext << ">" << endl; }
{BOOLEAN_CONSTANT}                  { cout << "BOOLEAN_CONSTANT <" << yytext << ">" << endl; }
{CHARACTER_CONSTANT}                { cout << "CHARACTER_CONSTANT <" << yytext << ">" << endl; }
{IDENTIFIER}                        { cout << "IDENTIFIER <" << yytext << ">" << endl; }
"/"                                 { cout << "SLASH <" << yytext << ">" << endl; }
"("                                 { cout << "LPARENT <" << yytext << ">" << endl; }
")"                                 { cout << "RPARENT <" << yytext << ">" << endl; }
";"                                 { cout << "SEMICOLON <" << yytext << ">" << endl; }
","                                 { cout << "COMMMA <" << yytext << ">" << endl; }
"="                                 { cout << "EQUALS <" << yytext << ">" << endl; }
" "                                 { /* cout << "WHITESPACE <" << yytext << ">" << endl; */}
.                                   { cout << "UNKNOWN <" << yytext << ">" << endl; }
%%
/*
int main(int argc, char ** argv) {
    yylex();
    return 0;
} */

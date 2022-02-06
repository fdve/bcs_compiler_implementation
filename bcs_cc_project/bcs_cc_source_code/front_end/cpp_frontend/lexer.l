%{
    #include <iostream>
    #include <sstream>
    #include "parser.tab.h"

    using namespace std;

    extern "C" {
        int yylex(void);
    }
%}

%option noyywrap

TYPE                bool|int|char|float|string
CONDITIONAL         If|For|While|Return|Continue
UNARY_OPERATOR      "!"|"-"
ARITHMETIC_OPERATOR "+"|"-"|"*"|"%"
RELATIONAL_OPERATOR "<"|">"|"<="|">="
EQUALITY_OPERATOR   "=="|"!="
LOGICAL_OPERATOR    "&&"|"||"
INTEGER_CONSTANT    [0-9]+
FLOAT_CONSTANT      ([0-9]+"."[0-9]+)|("."[0-9]+)
BOOLEAN_CONSTANT    true|false
CHARACTER_CONSTANT  ("'"[0-9a-zA-Z]"'")
IDENTIFIER          [a-zA-Z]([a-zA-Z0-9])*
LPARENT             "("
RPARENT             ")"



%%
{TYPE}                              { cout << "TYPE <" << yytext << ">" << endl; }
{CONDITIONAL}                       { cout << "CONDITIONAL <" << yytext << ">" << endl; }

{ARITHMETIC_OPERATOR}               { cout << "ARITHMETIC_OPERATOR <" << yytext << ">" << endl; return(ARITHMETIC_OPERATOR); }
{RELATIONAL_OPERATOR}               { cout << "RELATIONAL_OPERATOR <" << yytext << ">" << endl; return(RELATIONAL_OPERATOR);}
{LOGICAL_OPERATOR}                  { cout << "LOGICAL_OPERATOR <" << yytext << ">" << endl; return(LOGICAL_OPERATOR);  }
{EQUALITY_OPERATOR}                 { cout << "EQUALITY_OPERATOR <" << yytext << ">" << endl; return(EQUALITY_OPERATOR);   }

{UNARY_OPERATOR}                    { cout << "UNARY_OPERATOR <" << yytext << ">" << endl; }

{INTEGER_CONSTANT}                  { cout << "INTEGER_CONSTANT <" << yytext << ">" << endl; return(INTEGER_CONSTANT); }
{FLOAT_CONSTANT}                    { cout << "FLOAT_CONSTANT <" << yytext << ">" << endl;  return(FLOAT_CONSTANT); }
{BOOLEAN_CONSTANT}                  { cout << "BOOLEAN_CONSTANT <" << yytext << ">" << endl;  return(BOOLEAN_CONSTANT); }
{CHARACTER_CONSTANT}                { cout << "CHARACTER_CONSTANT <" << yytext << ">" << endl;  return(CHARACTER_CONSTANT); }

{IDENTIFIER}                        { cout << "IDENTIFIER <" << yytext << ">" << endl; return(IDENTIFIER); }
"/"                                 { cout << "SLASH <" << yytext << ">" << endl; }
"("                                 { cout << "LPARENT <" << yytext << ">" << endl; return(LPARENT);  }
")"                                 { cout << "RPARENT <" << yytext << ">" << endl; return(RPARENT);  }
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

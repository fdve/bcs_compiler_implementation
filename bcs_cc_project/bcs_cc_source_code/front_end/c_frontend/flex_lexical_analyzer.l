/*** Definition Section has one variable
     which can be accessed inside yylex()
     and main() ***/
%{
int count = 0;
%}

DIGIT [0-9]*
ID [a-z][a-z0-9]*
FUNCTIONBLOCK [{][A]*[}]



/* Rule Section */

%%

{DIGIT}             { printf( "An digit : %s\n", yytext ); }
{ID}                { printf( "An identifier: %s\n", yytext ); }
{FUNCTIONBLOCK}     { printf( "An functionblock : %s\n", yytext ); }
\n                  { return 0; }

%%


/* %%

"+"               { cout << "operator <" << yytext[0] << ">" << endl; }
"-"               { cout << "operator <" << yytext[0] << ">" << endl; }
"="               { cout << "operator <" << yytext[0] << ">" << endl; }
{DIGIT1}{DIGIT}*  { cout << "  number <" << yytext    << ">" << endl; }
.                 { cout << " UNKNOWN <" << yytext[0] << ">" << endl; }

%% */



/*
#FUNCTIONBLOCK {{1}[aaaa]}{1}



DIGIT [0-9]


[A-Z] { printf("%s capital letter\n", yytext); count++; }
.     { printf("%s not a capital letter\n", yytext); }
\n    { return 0; }

{DIGIT}+ { printf( "An integer: %s (%d)\n", yytext, atoi( yytext ) ); }
[DIGIT] { printf("%s hey, we found a digit! pog", yytext); }
[A-Z] { printf("%s testtest", yytext) ; }


/* [A-Z]       { printf("%s capital letter\n", yytext); count++; }
.           { printf("%s not a capital letter\n", yytext); }


*/




/*** Code Section prints the number of
capital letter present in the given input***/

// Explanation:
// yywrap() - wraps the above rule section
/* yyin - takes the file pointer
          which contains the input*/
/* yylex() - this is the main flex function
          which runs the Rule Section*/
// yytext is the text in the buffer


int yywrap() {}

int main() {
    yylex();
    printf("\nNumber of Capital letters in the given input - %d\n", count);
    return 0;
}


// Uncomment the lines below
// to take input from file
// FILE *fp;
// char filename[50];
// printf("Enter the filename: \n");
// scanf("%s",filename);
// fp = fopen(filename,"r");
// yyin = fp;

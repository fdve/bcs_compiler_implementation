%{
    #include <iostream>
    #include <sstream>
    using namespace std;

    extern "C" {
        int yylex(void);
        int yyerror(string);
    }

    void yyerror(const char *s) {
        fprintf(stderr, "error: %s\n", s);
    }
%}



// Symbols.
%union
{
	char	*sval;
};
%token <sval> IDENTIFIER
%token PROCEDURE
%token BLOCK
%token ENDBLOCK

%start Procedure
%%

Procedure:
	PROCEDURE IDENTIFIER BLOCK { printf("Procedure : %s\n", $2); }
	Parts
	ENDBLOCK
	;

Parts:
	/* empty */
	| Parts Part
	;

Part:
	IDENTIFIER BLOCK { printf("\tPart : %s\n", $1); }
		Keywords
	ENDBLOCK
	;

Keywords:
	/* empty */
	| Keywords Keyword
	;

Keyword:
	IDENTIFIER { printf("\t\tKeyword : %s\n", $1); }
	;
%%

int main(int argc, char **argv) {
    yyparse();
}

%{
    #include <iostream>
    #include <sstream>
    #include <vector>

    using namespace std;

    extern "C" {
        int yylex(void);
        int yyerror(string);
    }

    void yyerror(const char *s) {
        fprintf(stderr, "parser.y :     error: %s\n", s);
    }

    void parsed_rule(int i) {
        switch(i) {
            case 1:
                cout << "parsed expression!" << endl;
                break; //optional
            case 2:
                cout << "parsed bitwise_operator!" << endl;
                break; //optional
        }
    }
%}

%token IDENTIFIER
%token CONDITIONAL

%token UNARY_OPERATOR

%token ARITHMETIC_OPERATOR
%token RELATIONAL_OPERATOR
%token EQUALITY_OPERATOR
%token LOGICAL_OPERATOR

%token LPARENT
%token RPARENT

%token INTEGER_CONSTANT
%token FLOAT_CONSTANT
%token BOOLEAN_CONSTANT
%token CHARACTER_CONSTANT



/* %start EXPRESSION */
%%

INPUT :
    /* No tokens left? */
    | INPUT EXPRESSION;


EXPRESSION :
      UNARY_OPERATOR EXPRESSION { parsed_rule(1); }
    | EXPRESSION BITWISE_OPERATOR EXPRESSION { cout << "parsed expression type 2" << endl; }
    | LPARENT EXPRESSION RPARENT {cout << "parsed expression type 3" << endl;}
    | CONSTANT {cout << "parsed expression type 4" << endl; };

BITWISE_OPERATOR :
       ARITHMETIC_OPERATOR { parsed_rule(2); }
     | RELATIONAL_OPERATOR { parsed_rule(2); }
     | LOGICAL_OPERATOR { parsed_rule(2); }
     | EQUALITY_OPERATOR { parsed_rule(2); } ;

CONSTANT :
       INTEGER_CONSTANT
     | FLOAT_CONSTANT
     | BOOLEAN_CONSTANT
     | CHARACTER_CONSTANT;


%%

/* | EXPRESSION BITWISE_OPERATOR EXPRESSION { }; */



int main(int argc, char **argv) {
    /* vector<int> v = { 7, 5, 16, 8 };
    for(int i = 0; i < v.size(); i++) {
        cout << "v[i] = " << v[i] << endl;
    } */

    cout << "int main() in parser called!" << endl;
    yyparse();
    cout << "End of parsing." << endl;
}

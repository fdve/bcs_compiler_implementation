/* Bison parser file, where the tokens from Flex are being matched against the
 * parser productions defined here. Some rules have semantic actions, meaning that if
 * a certain parser rule is matched, the code in its body is executed. */

%{
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
%}

%locations
%define parse.trace

/* Need to add an union, otherwise YY_TYPE errors will occur.
 *
 * Also, for our own defined types, such as an expression,
 * declare a pointer towards them, otherwise also YYSTYPE will occur
 * because of a non-trivial union member. */

%union {
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
}

/* Defining are terminals as tokens with their corresponding types. */

%token INTEGER_CONSTANT FLOAT_CONSTANT NEGATION
%token <token> CF_IF CF_ELSE CF_FOR CF_WHILE CF_DO CF_RETURN
%token <token> LPARENT RPARENT LCURLYBRACE RCURLYBRACE
%token <token> SEMICOLON COMMA
%token <funcDecl> START_FUNC_DECL
%token <BoolConst> BOOLEAN_CONSTANT
%token <CharConst> MINUS
%token <string> LINEAR_ARITHMETIC PLUS  DATATYPE VOID IDENTIFIER CHARACTER_CONSTANT
%token <string> RELATIONAL_OPERATOR AND_OPERATOR OR_OPERATOR EQUALITY_OPERATOR ASSIGN MODIFIER

/* Associativity rules we give to our tokens and rules.
 * Precendence is higher for rules later declared, i.e. on higher line numbers. */


%right DEC_FUNC_PREC
%left DEC_FUNC_PREC2

%left OR_OPERATOR
%left AND_OPERATOR
%left EQUALITY_OPERATOR
%left RELATIONAL_OPERATOR
%left PLUS MINUS
%left LINEAR_ARITHMETIC
%right UNARY_OPERATOR_PREC





/* All the types we are giving to our non terminals . */
%type <string> RETURNTYPE
%type <IntConst> INTEGER_CONSTANT
%type <FloatConst> FLOAT_CONSTANT
%type <CharConst> NEGATION UNARY_OPERATOR

%type <stmntList>  STATEMENTS CONTROL_FLOW_OPT_ELSE
%type <stmnt> STATEMENT ASSIGNMENT VARIABLE_DEF EXPRESSION_SC RETURN_CALL CONTROL_FLOW FUNCTION_BLOCK

%type <expr> CONSTANT EXPRESSION FUNCTION_CALL RETURN_CALL_OPT
%type <exprList> FUNC_CALL_PARAM FUNC_CALL_PARAM_OPT

%type <funcDecl> DEC_FUNC START_FUNCTION
%type <_funcDeclList> DEC_FUNCS
%type <funcDeclParamsList> DEC_FUNC_CALL_PARAMS DEC_FUNC_CALL_PARAMS_OPT

%%

INPUT :
     /* Empty. */
    | INPUT PROGRAM
    ;

/* Every program should contain one int start() method without parameters.
 * - [x] Start functions location can be anywhere in the file? -> they can now but we have some SR conflicts i do not understand
                -> something else we could do is removing the start function rule, and during
                   semantic analysis check if we have a (correct) start function ?
 */
PROGRAM :
    {
        Scope::PushScope();
    }
    DEC_FUNCS START_FUNCTION DEC_FUNCS  %prec DEC_FUNC_PREC2
    {
        ASTRoot = new Program($2, $3, $4);
        Scope::PopScope();
    }


    // DEC_FUNCS                   { ASTRoot = new Program($1, nullptr, nullptr); }
    ;

/* -[] Add the start function to the sym table? */
START_FUNCTION :
    START_FUNC_DECL {
                                        Scope::AddSymbol("int", "start", &@1);
                                        currentFunc = "start";

                    }

        LPARENT RPARENT FUNCTION_BLOCK
    // START_FUNC_DECL LPARENT RPARENT LCURLYBRACE STATEMENTS RCURLYBRACE
    {
        Identifier *idNode = new Identifier("int", "start", Scope::GetSymbol("start"));
        $$ = new FuncDecl("int", idNode, nullptr, dynamic_cast<FunctionBlock *>($5)->stmnts, &@1);


    }
    ;


DEC_FUNCS :
    /* Empty. */                                        { $$ = new funcDeclList(); }
    | DEC_FUNCS DEC_FUNC       %prec DEC_FUNC_PREC2                         { $$->push_back($2); }

DEC_FUNC :
/* -[] NOT really a variable what we are adding here, but a func declaration (sym table -> add var) */
    RETURNTYPE IDENTIFIER  {
                                    Scope::AddSymbol(*$1, *$2, &@2);
                                    Scope::PushScope();
                                    currentFunc = *$2;


                                    // if identifier was a node instead, at this point we could enter the returned ref
                                    // from the add to scope func call?
                                    // or just do getSymbol below? :)
                            }


    LPARENT DEC_FUNC_CALL_PARAMS RPARENT FUNCTION_BLOCK
 %prec DEC_FUNC_PREC2

    {
        // // cout_str("PRINTING SCOPES AFTER PARSING FUNCTION");
        // Scope::PrintScopes(Scope::globalScope, 0);



        Identifier *idNode = new Identifier(*$1, *$2, Scope::GetSymbol(*$2));
        $$ = new FuncDecl(*$1, idNode, $5, dynamic_cast<FunctionBlock *>($7)->stmnts, &@1);

        Scope::GetSymbol(*$2)->params = $5;

        // symTableEntry *entry = Scope::GetSymbol(*$2);
        // SymbolTable::PrintEntry(*entry, 123);




        Scope::PopScope();
    }
    ;

DEC_FUNC_CALL_PARAMS :
    /* Empty. */                                        { $$ = nullptr;  }
    | RETURNTYPE IDENTIFIER DEC_FUNC_CALL_PARAMS_OPT
    {
        $3->push_back(new FuncDeclParam(*$1, new Identifier(*$1, *$2, Scope::AddSymbol(*$1, *$2, &@2))));
        $$ = $3;
        std::reverse($$->begin(), $$->end());
        ;
    }
    ;

DEC_FUNC_CALL_PARAMS_OPT :
    /* Empty. */                                        { $$ = new funcDeclParamList(); }
    | COMMA RETURNTYPE IDENTIFIER DEC_FUNC_CALL_PARAMS_OPT
    {
        $4->push_back(new FuncDeclParam(*$2, new Identifier(*$2, *$3, Scope::AddSymbol(*$2, *$3, &@3))));
        $$ = $4;
    }
    ;

RETURNTYPE :
      DATATYPE                                          { $$ = $1; }
    | VOID                                              { $$ = $1; }
    ;


/* Since a function block can return multiple statements in them, it is convenient
 * to place the function block parse rule under statements instead of under statement, so
 * our statement rules return a singular statement node and function block a vector of statements.
*/
STATEMENTS :
      /* Empty. */                                      { $$ = new stmntList(); }
    | STATEMENTS STATEMENT
    {
        if(CFDoWhile *doWhile = dynamic_cast<CFDoWhile*>($2)) {
            $2 = desugarDoWhile($$, doWhile);
        }
        else if(CFFor *forNode = dynamic_cast<CFFor*>($2)) {
            $2 = desugarFor($$, forNode);
        }

        if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>($2)) {
            for(Statement *stmnt : *funcBlock->stmnts) {
                $$->push_back(stmnt);
            }
        }
        else {
            $$->push_back($2);
        }
    }
    ;

STATEMENT :
      VARIABLE_DEF                                    { $$ = $1; }
    | ASSIGNMENT                                      { $$ = $1; }
    | EXPRESSION_SC                                   { $$ = $1; }
    | RETURN_CALL                                     { $$ = $1; }
    | CONTROL_FLOW                                    { $$ = $1; }
    | FUNCTION_BLOCK                                  { $$ = $1; }
    ;

FUNCTION_BLOCK :
    {
        Scope::PushScope();
    }
    LCURLYBRACE STATEMENTS RCURLYBRACE
    {
        $$ = new FunctionBlock($3);
        Scope::PopScope();
    }
    ;
// CHECKEN OF WE WEL VARS TOEVOEGEN AAN SYM TABLE !
VARIABLE_DEF :
    DATATYPE IDENTIFIER ASSIGN EXPRESSION SEMICOLON
    {
        $$ = new VariableDef(*$1, new Identifier(*$1, *$2, Scope::AddSymbol(*$1, *$2, &@2)), $4, &@2);
        // cout_str("VAR DEF CHECKING column " + *$2);
        // cout_str(std::to_string(@2.first_column));
    }
    ;

ASSIGNMENT :
      IDENTIFIER ASSIGN EXPRESSION SEMICOLON
      {
          if(Scope::ContainsKey(*$1)) {
              $$ = new Assignment(new Identifier("", *$1, Scope::GetSymbol(*$1)), "=", $3, &@1);
          }
          else {

              // Scope::PrintScopes(Scope::globalScope, 0);
              $$ = new Assignment(new Identifier(*$1), "=", $3, &@1);
              ErrorLogger::InsertMsg(ErrorLogger::UndefinedVar(newInfo(&@1), *$1));
          }
      }

    | IDENTIFIER MODIFIER EXPRESSION SEMICOLON
    {
        if(Scope::ContainsKey(*$1)) {
            $$ = new Assignment(new Identifier("", *$1, Scope::GetSymbol(*$1)), *$2, $3, &@1);
        }
        else {
            $$ = new Assignment(new Identifier(*$1), *$2, $3, &@1);
            ErrorLogger::InsertMsg(ErrorLogger::UndefinedVar(newInfo(&@1), *$1));
        }
    }
    ;

EXPRESSION_SC :
    EXPRESSION SEMICOLON                              { $$ = new ExprStmnt($1); }
    ;

RETURN_CALL :
    CF_RETURN RETURN_CALL_OPT SEMICOLON               { $$ = new ReturnCall($2, new Identifier("", currentFunc, Scope::GetSymbol(currentFunc)), &@2); }
    ;

RETURN_CALL_OPT :
      /* Empty. */                                    { $$ = nullptr; }
    | EXPRESSION                                      { $$ = $1; }
    ;

CONTROL_FLOW :
      CF_IF EXPRESSION LCURLYBRACE { Scope::PushScope(); } STATEMENTS { Scope::PopScope(); }  RCURLYBRACE CONTROL_FLOW_OPT_ELSE { $$ = new CFIf($2, $5, $8, &@2); }
    | CF_FOR { Scope::PushScope(); } VARIABLE_DEF EXPRESSION SEMICOLON ASSIGNMENT LCURLYBRACE STATEMENTS RCURLYBRACE            { Scope::PopScope(); $$ = new CFFor($3, $4, $6, $8, &@4); }
    | CF_WHILE EXPRESSION LCURLYBRACE { Scope::PushScope(); } STATEMENTS { Scope::PopScope(); } RCURLYBRACE                     { $$ = new CFWhile($2, $5, &@2); }
    | CF_DO LCURLYBRACE  { Scope::PushScope(); } STATEMENTS { Scope::PopScope(); } RCURLYBRACE CF_WHILE EXPRESSION SEMICOLON    { $$ = new CFDoWhile($8, $4, &@8); }
    ;

CONTROL_FLOW_OPT_ELSE :
      /* Empty. */                                   { $$ = nullptr; }
    |  CF_ELSE LCURLYBRACE STATEMENTS RCURLYBRACE    { $$ = $3; }
    ;

EXPRESSION :
    /* un_op and bitwise rules causes 5 sr  conflicts. */
    UNARY_OPERATOR EXPRESSION                         %prec UNARY_OPERATOR_PREC
                                                      { $$ = new UnaryOpExpr($1, $2, &@1); }
    | EXPRESSION LINEAR_ARITHMETIC EXPRESSION         { $$ = new BitwiseOpExpr($1, *$2, $3, &@2); }
    | EXPRESSION PLUS EXPRESSION                      { $$ = new BitwiseOpExpr($1, *$2, $3, &@2); }
    | EXPRESSION MINUS EXPRESSION                     { $$ = new BitwiseOpExpr($1, std::string("-"), $3, &@2); }
    | EXPRESSION RELATIONAL_OPERATOR EXPRESSION       { $$ = new BitwiseOpExpr($1, *$2, $3, &@2); }
    | EXPRESSION AND_OPERATOR EXPRESSION              { $$ = new BitwiseOpExpr($1, *$2, $3, &@2); }
    | EXPRESSION OR_OPERATOR EXPRESSION               { $$ = new BitwiseOpExpr($1, *$2, $3, &@2); }
    | EXPRESSION EQUALITY_OPERATOR EXPRESSION         { $$ = new BitwiseOpExpr($1, *$2, $3, &@2); }
    | LPARENT EXPRESSION RPARENT                      { $$ = $2; }
    | FUNCTION_CALL                                   { $$ = $1; }
    | IDENTIFIER
    {
        if(Scope::ContainsKey(*$1)) {
            $$ = new Identifier(Scope::GetSymbol(*$1));

            /* -[] everything above seems to work. Why does the ref encoutnerd in print visitor is diffierent?
              Something to do with pushing / popping scopes? */
        }
        else {
            	/* -[] shouldnt this give an error? unknown ID? <- think the scope contains call already does that*/
            $$ = new Identifier(*$1, &@1);
        }

    }
    | CONSTANT                                      { $$ = $1; }
    ;

UNARY_OPERATOR :
      MINUS                                         { $$ = $1; }
    | NEGATION                                      { $$ = $1; }
    ;

FUNCTION_CALL :
    IDENTIFIER LPARENT FUNC_CALL_PARAM RPARENT
    {
        if($3 != nullptr) {
            std::reverse($3->begin(), $3->end());
        }
        // we add the type of the function call by looking up
        // the ID in the existing scopes

        if(Scope::ContainsKey(*$1)) {
            // cout_str("EXPR : FUNCTION_CALL in parser.y");
            // cout_str("EXPR : FUNCTION_CALL checkign if entry has params set : ");
            // cout_str("EXPR : FUNCTION_CALL identifier : " + *$1);

            symTableEntry *entry = Scope::GetSymbol(*$1);

            if(entry->params == nullptr) {
                // cout_str("EXPR : FUNCTION_CALL entry->params == nullptr");
            }

            $$ = new FuncCall(entry->type, new Identifier(entry), $3, &@1);
        }
        else {
            $$ = new FuncCall("unknown_return_type", new Identifier(*$1), $3, &@1);
        }
    }
    ;

FUNC_CALL_PARAM :
      /* Empty. */                                  { $$ = nullptr; }
    | EXPRESSION FUNC_CALL_PARAM_OPT                { $2->push_back($1); $$ = $2; }
    ;

FUNC_CALL_PARAM_OPT :
    /* Empty. */                                    { $$ = new exprList(); }
    | COMMA EXPRESSION FUNC_CALL_PARAM_OPT          { $3->push_back($2); $$ = $3; }
    ;

CONSTANT :
      INTEGER_CONSTANT                              { $$ = new IntConst($1); }
    | FLOAT_CONSTANT                                { $$ = new FloatConst($1); }
    | BOOLEAN_CONSTANT                              { $$ = new BoolConst($1); }
    | CHARACTER_CONSTANT                            { $$ = new CharConst(*$1); }
    ;

%%

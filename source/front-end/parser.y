/* Bison parser file, where the tokens from Flex are being matched against the
 * parser productions defined here. Some rules have semantic actions, meaning that if
 * a certain parser rule is matched, the code in its body is executed,
 * for instance, when a assignment has been parsed, return a new assignment node. */

%{
    #include <iostream>
    #include <vector>
    #include <algorithm>

    #include "../headers/ast.h"
    #include "../headers/astvisitor.h"
    #include "../headers/desugar.h"
    #include "../headers/util.h"
    #include "../headers/symboltable.h"
    #include "../headers/scope.h"
    #include "../headers/errorlogger.h"

    #define YYERROR_VERBOSE 1

    ASTNode *ASTRoot;

    extern int yylex();
    extern int yylineno;
    extern int yyloc_first_col_cpy;

    /* Keep track of which function is currently being parsed, so that return call
     * nodes can be annotated with the name of which function they return a value to. */
    std::string currentFunc = "";

    /* Whenever a parsing error occurs, yyerror will be called. */
    void yyerror(std::string errorMsg) {
        Scope::ClearScopes();
        std::string currentFunc = "";
        ErrLog::parsingError = true;
        ErrLog::InsertMsg(ErrLog::ParserError(yylineno, yyloc_first_col_cpy, errorMsg));
    }
%}

%locations
%define parse.trace

/* Add the classes which are being used to the union. */
%union {
    int IntConst;
    float FloatConst;
    char CharConst;
    bool BoolConst;

    std::string *string;

    FuncDecl *funcDecl;
    FuncDeclParam *funcDeclParam;
    FuncDeclParamList *funcDeclParamList;
    FuncDeclList *funcDeclList;

    Expression *expr;
    Statement *stmnt;

    std::vector<Expression *> *exprList;
    std::vector<Statement *> *stmntList;
    Identifier *ident;

    ASTNode *astNode;
}

/* Defining terminals as tokens with their corresponding types. */
%token <token> CF_IF CF_ELSE CF_FOR CF_WHILE CF_DO CF_RETURN
%token <token> LPARENT RPARENT LCURLYBRACE RCURLYBRACE SEMICOLON COMMA
%token <IntConst> INTEGER_CONSTANT
%token <FloatConst> FLOAT_CONSTANT
%token <CharConst> MINUS NEGATION
%token <BoolConst> BOOLEAN_CONSTANT
%token <funcDecl> START_FUNC_DECL
%token <string> LINEAR_ARITHMETIC PLUS  DATATYPE VOID IDENTIFIER CHARACTER_CONSTANT
%token <string> RELATIONAL_OPERATOR AND_OPERATOR OR_OPERATOR EQUALITY_OPERATOR ASSIGN MODIFIER

/* Associativity and precedence rules for the tokens and rules.
 * Precendence is higher for rules later declared, i.e. on higher line numbers. */
%left OR_OPERATOR
%left AND_OPERATOR
%left EQUALITY_OPERATOR
%left RELATIONAL_OPERATOR
%left PLUS MINUS
%left LINEAR_ARITHMETIC
%right UNARY_OP_PREC

/* All the types we are giving to our non terminals . */
%type <string> RETURNTYPE
%type <CharConst> UNARY_OPERATOR
%type <stmntList>  STATEMENTS CF_OPT_ELSE
%type <stmnt> STATEMENT ASSIGNMENT VARIABLE_DEF EXPRESSION_SC RETURN_CALL CONTROL_FLOW FUNCTION_BLOCK
%type <expr> CONSTANT EXPRESSION FUNCTION_CALL RETURN_CALL_OPT
%type <exprList> FUNC_CALL_PARAM FUNC_CALL_PARAM_OPT
%type <funcDecl> DEC_FUNC START_FUNCTION
%type <funcDeclList> DEC_FUNCS
%type <funcDeclParamList> DEC_FUNC_CALL_PARAMS DEC_FUNC_CALL_PARAMS_OPT

%%

INPUT :
     /* Empty. */
    | INPUT PROGRAM
    ;

PROGRAM :
    {
        Scope::PushScope();
    }
    DEC_FUNCS START_FUNCTION DEC_FUNCS
    {
        ASTRoot = new Program($2, $3, $4);
        Scope::PopScope();
    }
    ;

START_FUNCTION :
    START_FUNC_DECL
    {
        Scope::AddSymbol("int", "start", &@1);
        currentFunc = "start";
    }
    LPARENT RPARENT FUNCTION_BLOCK
    {
        Identifier *idNode = new Identifier("int", "start", Scope::GetSymbol("start"));
        $$ = new FuncDecl("int", idNode, nullptr, dynamic_cast<FunctionBlock *>($5)->GetStmnts(), &@1);
    }
    ;

DEC_FUNCS :
    /* Empty. */
    {
        $$ = new FuncDeclList();
    }
    | DEC_FUNCS DEC_FUNC
    {
        $$->push_back($2);
    }

DEC_FUNC :
    RETURNTYPE IDENTIFIER
    {
        Scope::AddSymbol(*$1, *$2, &@2);
        Scope::PushScope();
        currentFunc = *$2;
    }
    LPARENT DEC_FUNC_CALL_PARAMS RPARENT FUNCTION_BLOCK
    {
        Identifier *idNode = new Identifier(*$1, *$2, Scope::GetSymbol(*$2));
        $$ = new FuncDecl(*$1, idNode, $5, dynamic_cast<FunctionBlock *>($7)->GetStmnts(), &@1);

        Scope::GetSymbol(*$2)->params = $5;
        Scope::PopScope();
    }
    ;

DEC_FUNC_CALL_PARAMS :
    /* Empty. */
    {
        $$ = nullptr;
    }
    | RETURNTYPE IDENTIFIER DEC_FUNC_CALL_PARAMS_OPT
    {
        $3->push_back(new FuncDeclParam(*$1, new Identifier(*$1, *$2, Scope::AddSymbol(*$1, *$2, &@2))));
        $$ = $3;
        std::reverse($$->begin(), $$->end());
    }
    ;

DEC_FUNC_CALL_PARAMS_OPT :
    /* Empty. */                                        { $$ = new FuncDeclParamList(); }
    | COMMA RETURNTYPE IDENTIFIER DEC_FUNC_CALL_PARAMS_OPT
    {
        $4->push_back(new FuncDeclParam(*$2, new Identifier(*$2, *$3, Scope::AddSymbol(*$2, *$3, &@3))));
        $$ = $4;
    }
    ;

RETURNTYPE :
    DATATYPE                                            { $$ = $1; }
    | VOID                                              { $$ = $1; }
    ;

STATEMENTS :
      /* Empty. */                                      { $$ = new stmntList(); }
    | STATEMENTS STATEMENT
    {
        if(CFDoWhile *doWhile = dynamic_cast<CFDoWhile*>($2)) {
            $2 = Desugar::DesugarDoWhile($$, doWhile);
        }
        else if(CFFor *forNode = dynamic_cast<CFFor*>($2)) {
            $2 = Desugar::DesugarFor($$, forNode);
        }

        if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>($2)) {
            for(Statement *stmnt : *funcBlock->GetStmnts()) {
                $$->push_back(stmnt);
            }
        }
        else {
            $$->push_back($2);
        }
    }
    ;

STATEMENT :
      FUNCTION_BLOCK                                  { $$ = $1; }
    | VARIABLE_DEF                                    { $$ = $1; }
    | ASSIGNMENT                                      { $$ = $1; }
    | EXPRESSION_SC                                   { $$ = $1; }
    | RETURN_CALL                                     { $$ = $1; }
    | CONTROL_FLOW                                    { $$ = $1; }
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

VARIABLE_DEF :
    DATATYPE IDENTIFIER ASSIGN EXPRESSION SEMICOLON
    {
        $$ = new VariableDef(*$1, new Identifier(*$1, *$2, Scope::AddSymbol(*$1, *$2, &@2)), $4, &@2);
    }
    ;

ASSIGNMENT :
    IDENTIFIER ASSIGN EXPRESSION SEMICOLON
    {
        if(Scope::ContainsKey(*$1)) {
            $$ = new Assignment(new Identifier("", *$1, Scope::GetSymbol(*$1)), "=", $3, &@1);
        }
        else {
            $$ = new Assignment(new Identifier(*$1), "=", $3, &@1);
            ErrLog::InsertMsg(ErrLog::UndefinedVar(newInfo(&@1), *$1));
        }
    }
    | IDENTIFIER MODIFIER EXPRESSION SEMICOLON
    {
        if(Scope::ContainsKey(*$1)) {
            $$ = new Assignment(new Identifier("", *$1, Scope::GetSymbol(*$1)), *$2, $3, &@1);
        }
        else {
            $$ = new Assignment(new Identifier(*$1), *$2, $3, &@1);
            ErrLog::InsertMsg(ErrLog::UndefinedVar(newInfo(&@1), *$1));
        }
    }
    ;

EXPRESSION_SC :
    EXPRESSION SEMICOLON                              { $$ = new ExprStmnt($1); }
    ;

RETURN_CALL :
    CF_RETURN RETURN_CALL_OPT SEMICOLON
    {
        $$ = new ReturnCall($2, new Identifier("", currentFunc, Scope::GetSymbol(currentFunc)), &@2);
    }
    ;

RETURN_CALL_OPT :
      /* Empty. */                                    { $$ = nullptr; }
    | EXPRESSION                                      { $$ = $1; }
    ;

CONTROL_FLOW :
    CF_IF EXPRESSION LCURLYBRACE { Scope::PushScope(); } STATEMENTS { Scope::PopScope(); } RCURLYBRACE CF_OPT_ELSE
    {
        $$ = new CFIf($2, $5, $8, &@2);
    }
    | CF_FOR { Scope::PushScope(); } VARIABLE_DEF EXPRESSION SEMICOLON ASSIGNMENT LCURLYBRACE STATEMENTS RCURLYBRACE
    {
        Scope::PopScope();
        $$ = new CFFor($3, $4, $6, $8, &@4);
    }
    | CF_WHILE EXPRESSION LCURLYBRACE { Scope::PushScope(); } STATEMENTS { Scope::PopScope(); } RCURLYBRACE
    {
        $$ = new CFWhile($2, $5, &@2);
    }
    | CF_DO LCURLYBRACE  { Scope::PushScope(); } STATEMENTS { Scope::PopScope(); } RCURLYBRACE CF_WHILE EXPRESSION SEMICOLON
    {
        $$ = new CFDoWhile($8, $4, &@8);
    }
    ;

CF_OPT_ELSE :
      /* Empty. */                                    { $$ = nullptr; }
    |  CF_ELSE LCURLYBRACE STATEMENTS RCURLYBRACE     { $$ = $3; }
    ;

EXPRESSION :
    UNARY_OPERATOR EXPRESSION   %prec UNARY_OP_PREC   { $$ = new UnaryOpExpr($1, $2, &@1); }
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
        }
        else {
            $$ = new Identifier(*$1, &@1);
        }
    }
    | CONSTANT                                        { $$ = $1; }
    ;

UNARY_OPERATOR :
      MINUS                                           { $$ = $1; }
    | NEGATION                                        { $$ = $1; }
    ;

FUNCTION_CALL :
    IDENTIFIER LPARENT FUNC_CALL_PARAM RPARENT
    {
        if($3 != nullptr) {
            std::reverse($3->begin(), $3->end());
        }

        if(Scope::ContainsKey(*$1)) {
            symTableEntry *entry = Scope::GetSymbol(*$1);
            $$ = new FuncCall(entry->type, new Identifier(entry), $3, &@1);
        }
        else {
            $$ = new FuncCall("UnknownReturnType", new Identifier(*$1), $3, &@1);
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
    | CHARACTER_CONSTANT                            { $$ = new CharConst(*$1, &@1); }
    ;

%%

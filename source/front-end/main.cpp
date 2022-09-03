#include <iostream>
#include <string>

#include "../headers/ast.h"
#include "../headers/symboltable.h"
#include "../headers/astvisitors.h"
#include "../headers/util.h"
#include "../headers/typeresolution.h"

#include "../headers/main.h"
#include "../headers/errorlogger.h"

extern int yyparse();
extern int yydebug;
extern FILE *yyin;
extern int yylex();
extern int yy_flex_debug;

extern ASTNode *ASTRoot;
extern Expression *testNode;
extern SymbolTable *symbolTable;
extern void InitFlex();
// extern int yylex_destroy(void);
// yy_lex_destroy();

int main(int argc, char *argv[]) {
    InitFlex();

    ErrorLogger::Reset();
    CompileSourceFile(argc, (std::string)argv[1], CompilePhase::all);
    return 0;
}

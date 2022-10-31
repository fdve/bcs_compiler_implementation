#include <iostream>
#include <string>
#include <fstream>

#include "../headers/ast.h"
#include "../headers/main.h"
#include "../headers/symboltable.h"
#include "../headers/astvisitor.h"
#include "../headers/typevisitor.h"
#include "../headers/printvisitor.h"
#include "../headers/quadgenvisitor.h"
#include "../headers/util.h"
#include "../headers/typeresolution.h"
#include "../headers/quad.h"
#include "../headers/asmcodegen.h"
#include "../headers/errorlogger.h"

extern int yyparse();
extern int yydebug;
extern FILE *yyin;
extern int yylex();
extern int yy_flex_debug;
extern ASTNode *ASTRoot;

/* Use yy_flex_debug = 1 for debugging flex, use yydebug = 1 for debugging bison. */

/* Try to compile input file to asm at the default output file path. */
int CompileSourceFile(int argc, std::string inputFile, int compilePhase) {
    return CompileSourceFile(argc, inputFile, ASMCodeGenerator::defaultFilePath, compilePhase);
}

/* Try to compile input file to asm at the user provided output file path. */
int CompileSourceFile(int argc, std::string inputFile, std::string outputFile, int compilePhase) {
    if(argc == 1 || argc > 2) {
        cout_str("error : expected one argument (file to be compiled), got "
        + std::to_string(argc - 1) + " instead.");
        return 1;
    }

    SymbolTable::temporariesSymTab.clear();
    ErrLog::fileName = inputFile;

    if(ErrLog::trimFilePath) {
        std::string filePath = inputFile;
        auto pos = filePath.find_last_of('/');
        ErrLog::fileName = filePath.substr(pos + 1);
    }

    /* Check file extension. If incorrect, report error and return. */
    if(checkCorrectFileExt(inputFile) == false) {
        std::cout << "error : given file has incorrect file extension (expecting .star)." << '\n';
        return 1;
    }

    yyin = fopen(inputFile.c_str(), "r");
    if (!yyin) {
        std::cout << "error : could not open file " << inputFile << std::endl;
        return 1;
    }
    std::cout << "Compiling input file " << inputFile << "..." << std::endl;

    int parse_result = yyparse();

    /* If parsing failed, stop with with the compilation. The AST is built during parsing,
     * therefore also scope checking and type checking is not possible, since those checks
     * happen while constructing or traversing the tree.*/
    if(parse_result == 1) {
        ErrLog::PrintErrors();
        return 1;
    }

    PrintVisitor astPrintVisitor;
    TypeVisitor astTypeVisitor;
    QuadGenVisitor quadGenVisitor;

    /* Print the tree. */
    // ASTRoot->VisitASTNode(&astPrintVisitor);

    /* Print all the scopes. */
    // Scope::PrintScopes(Scope::globalScope, 0);

    ASTRoot->VisitASTNode(&astTypeVisitor);

    /* Checking whether there have been any errors during the processing
    *  of the source file so far. If there are, or the compilation flag has been
    *  set to stop compiling after the front-end, stop further compilation. */
    if(ErrLog::errorMsgs.size() != 0) {
        ErrLog::PrintErrors();
        return 1;
    }
    else if(compilePhase == CompilePhase::front) {
        return 0;
    }

    /* Visit the AST and generate TAC while traversing it. The generated Quads
     * will be stored in the Quad class static field Quad::QuadInstrucs. */
    ASTRoot->VisitASTNode(&quadGenVisitor);

    /* Printing out the generated Quad. */
    Quad::PrintQuadInstrucs(Quad::QuadInstrucs);

    if(ErrLog::errorMsgs.size() != 0) {
        ErrLog::PrintErrors();
        return 1;
    }
    else if(compilePhase == CompilePhase::front) {
        return 0;
    }

    /* Pass the generated Quad::QuadInstrucs to the x86_64 ASM generator,
     * and provide a file path where the generated asm code will be stored. */
    bool asmGenSuccess = ASMCodeGenerator::GenerateASMFromQuad();

    if(!asmGenSuccess) {
        std::cerr << "error : something went wrong while generating ASM from the supplied Quads.\n";
        return 1;
    }

    bool writeFileSucces = ASMCodeGenerator::GenerateAssemblyFile(outputFile);

    if(!writeFileSucces) {
        std::cerr << "error : something went wrong while writing ASM file.\n";
        return 1;
    }

    cout_str("Generated assembly successfully, written to file " + outputFile);

    return 0;
}

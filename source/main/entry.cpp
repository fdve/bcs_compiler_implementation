#include <iostream>
#include <string>
#include <fstream>

#include "../headers/ast.h"
#include "../headers/main.h"
#include "../headers/symboltable.h"
#include "../headers/astvisitors.h"
#include "../headers/util.h"
#include "../headers/typeresolution.h"
#include "../headers/tac.h"
#include "../headers/asmcodegen.h"
#include "../headers/tacoptimizer.h"
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
    SymbolTable::temporariesSymTab.clear();
    ErrorLogger::fileName = inputFile;

    if(ErrorLogger::trimFilePath) {
        std::string filePath = inputFile;
        auto pos = filePath.find_last_of('/');
        ErrorLogger::fileName = filePath.substr(pos + 1);
    }

    /* Check file extension. If incorrect, report error and return. */
    if(checkCorrectFileExt(inputFile) == false) {
        std::cout << "error : given file has incorrect file extension (expecting .star)." << '\n';
        return -1;
    }

    if(argc > 3 || argc == 0) {
        std::cout << "0 or more than 1 command line args given, reading from stdin..." << std::endl;
    }

    yyin = fopen(inputFile.c_str(), "r");
    if (!yyin) {
        std::cout << "error : could not open file " << inputFile << ", reading from std::in instead." << std::endl;
        ErrorLogger::fileName = "stdin";
    }

    std::cout << "Compiling input file " << inputFile << "..." << std::endl;

    int parse_result = yyparse();

    /* If parsing failed, stop with with the compilation. The AST is built during parsing,
     * therefore also scope checking and type checking is not possible, since those checks
     * happen while constructing or traversing the tree.*/
    if(parse_result == 1) {
        ErrorLogger::PrintErrors();
        return -1;
    }

    PrintVisitor astPrintVisitor;
    TypeVisitor astTypeVisitor;
    TACGenVisitor tacGenVisitor;

    // cout_str("\n--- PRINT VISITOR VISITING TREE ---");
    // ASTRoot->VisitASTNode(&astPrintVisitor);
    // cout_str("--- PRINT VISITOR DONE VISITING TREE ---");
    //
    // cout_str("\n--- PRINTING ALL SCOPES SYMBOL TABLES ---");
    // Scope::PrintScopes(Scope::globalScope, 0);

    // return 0;
    //
    // cout_str("\n--- TYPE VISITOR VISITING TREE ---");
    ASTRoot->VisitASTNode(&astTypeVisitor);
    // cout_str("--- TYPE VISITOR DONE VISITING TREE ---");

    // ASTRoot->VisitASTNode(&astPrintVisitor);

    /* Checking whether there have been any errors during the processing
    *  of the source file so far. If there are, or the compilation flag has been
    *  set to stop compiling after the front-end, stop further compilation. */
    if(ErrorLogger::errorMsgs.size() != 0) {
        ErrorLogger::PrintErrors();
        // Scope::PrintScopes(Scope::globalScope, 0);
        return -1;
    }
    else if(compilePhase == CompilePhase::front) {
        return 0;
    }

    /* Visit the AST and generate TAC while traversing it. The generated TAC
     * will be stored in the TAC class static field tacInstructions. */
    cout_str("\n--- TAC Gen visiting tree... ---");
    ASTRoot->VisitASTNode(&tacGenVisitor);
    cout_str("--- TAC Gen visitor done, printing TAC... ---");

    /* Printing out the generated TAC. */
    TAC::PrintTACInstructions(TAC::tacInstructions);
    // cout_str("num of tac instructions = " + std::to_string(TAC::tacInstructions.size()));

    if(ErrorLogger::errorMsgs.size() != 0) {
        ErrorLogger::PrintErrors();
        // Scope::PrintScopes(Scope::globalScope, 0);
        return -1;
    }
    else if(compilePhase == CompilePhase::front) {
        return 0;
    }


    /* Pass the generated tacInstructions to a TAC to x86_64 ASM converter,
     * and provide a file path where the generated code will be stored. */
    bool asmGenSuccess = ASMCodeGenerator::GenerateASMFromTAC();

    if(!asmGenSuccess) {
        std::cerr << "error : something went wrong while generating ASM from the supplied TAC.\n";
        return -1;
    }

    bool writeFileSucces = ASMCodeGenerator::GenerateAssemblyFile(outputFile);

    std::cout << "outputfile = " << outputFile << '\n';

    if(!writeFileSucces) {
        std::cerr << "error : something went wrong while writing ASM file.\n";
        return -1;
    }

    cout_str("Written assembly successfully to " + outputFile);

    // std::string asmFilePath = "./asm_out/x86_64_asm_file.s";
    // std::cout << "\nPrinting out the generated asm, from file: " << asmFilePath << std::endl;

    /* Reading ASM file generated by converting TAC into x86_64 ASM. */
    // std::ifstream file_stream;
    // file_stream.open(asmFilePath);

    // if(!file_stream) {
    //     std::cerr << "error : could not open file " << asmFilePath << std::endl;
    //     return -1;
    // }

    // std::cout << file_stream.rdbuf();
    return 0;
}

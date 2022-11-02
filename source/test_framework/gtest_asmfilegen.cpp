/* Use the compiled compiler to generate x86_64 ASM for the given star input files. */

#include <iostream>
#include <vector>
#include <utility>

#include "../headers/main.h"
#include "../headers/util.h"
#include "../headers/errorlogger.h"
#include "../headers/symboltable.h"
#include "../headers/asmcodegen.h"
#include "../headers/ast.h"
#include "../headers/quad.h"

extern int yyparse();
extern FILE *yyin;
extern void InitFlex();

/* Generate the asm files which are going to be tested. */
int main(int argc, char *argv[]) {
    std::string inputFilePath = "./source/test_framework/test_files/back-end/";
    std::string outputFilePath = "./source/test_framework/test_files/gen_asm_files/";

    /* Test files which will return an int value. */
    std::vector<std::string> intFileNames = { "asm_op_prec", "asm_assign", "asm_neg", "asm_arithmetic", "asm_cf",
                                              "asm_call_simple1", "asm_call_simple2", "asm_call_bool", "asm_call_mixed"};

    /* Test files which return a float value. */
    std::vector<std::string> floatFileNames = { "asm_float_arithmetic", "asm_float_cf", "asm_float_neg",
                                                "asm_float_assign" };

    /* Store whether the test file is a test which returns an int or float value,
     * since they are handled differently. Int tests will return their value in main,
     * while float test will return their value in a seperately float defined function.
     * Cant return it in main (start), since that function returns int, and cannot
     * remove the start function or the parsing of the source file would not succeed.
     * (int start() is mandatory in the parsing syntax). */
	std::vector<std::pair<std::string, bool>> filesWithType;

    for(size_t i = 0; i < intFileNames.size(); i++) {
        std::pair<std::string, bool> tempPair (intFileNames[i], false);
        filesWithType.push_back(tempPair);
    }

    for(size_t i = 0; i < floatFileNames.size(); i++) {
        std::pair<std::string, bool> tempPair (floatFileNames[i], true);
        filesWithType.push_back(tempPair);
    }

    size_t numOfTests = intFileNames.size() + floatFileNames.size();

    /* Compile the source files into asm. */
    for(size_t i = 0; i < numOfTests ; i++) {
        Quad::QuadInstrucs.clear();
        ErrLog::Reset();
        InitFlex();
        std::string tempInputFilePath = inputFilePath + filesWithType.at(i).first + ".star";
        std::string tempOutputFilePath = outputFilePath + filesWithType.at(i).first + ".s";

        std::cout << tempOutputFilePath << std::endl;

        if(!filesWithType.at(i).second) {
            ASMCodeGenerator::setMainLabelToEntry = true;
            ASMCodeGenerator::SetEntryPoint("test_" + filesWithType.at(i).first);
        }
        else {
            ASMCodeGenerator::setMainLabelToEntry = false;
            ASMCodeGenerator::SetEntryPoint("test_" + filesWithType.at(i).first);

        }
        compFlags flags;
        flags.fileDest = tempOutputFilePath;
        flags.customFileDest = true;

        CompileSourceFile(2, tempInputFilePath, CompilePhase::all, flags);
    }
}

#include <iostream>
#include <string>

#include "../headers/ast.h"
#include "../headers/symboltable.h"
#include "../headers/astvisitor.h"
#include "../headers/util.h"
#include "../headers/typeresolution.h"

#include "../headers/main.h"
#include "../headers/errorlogger.h"

extern void InitFlex();

/* Main function for star compiler which tries to compile Star source programs
 * int x86_64 assembly. */
int main(int argc, char *argv[]) {
    InitFlex();
    ErrLog::Reset();

    if(argc == 1 || argc > 2) {
        cout_str("error : expected one argument (file to be compiled), got "
        + std::to_string(argc-1) + " instead.");
        return 1;
    }

    CompileSourceFile(argc, (std::string)argv[1], CompilePhase::all);
    return 0;
}

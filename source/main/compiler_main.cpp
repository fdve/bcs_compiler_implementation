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

    if(argc == 1 || argc > 5) {
        cout_str("error : unexpected amount of command line args (" + std::to_string(argc - 1)
        + "), expected between 2 - 4");
        cout_str(cmmndLineArgsUsage);
        return 1;
    }

    if(argc == 2) {
        CompileSourceFile(argc, (std::string)argv[1], CompilePhase::all);
    }
    else {
        compFlags flags = ParseCmmndLineArgs(argc, argv);
        if(flags.invalidFlag) {
            cout_str(cmmndLineArgsUsage);
            return 1;
        }
        else {
            CompileSourceFile(argc, (std::string)argv[1], CompilePhase::all, flags);
        }
    }

    return 0;
}

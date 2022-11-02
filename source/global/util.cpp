/* File containing some utility functions for the compiler. */

#include <sstream>
#include <string>

#include "../headers/ast.h"
#include "../headers/util.h"
#include "../headers/parser.hpp"
#include "../headers/main.h"

/* Allocate a pointer in the heap and set its contents to given YYLTYPE info struct. */
YYLTYPE* newInfo(YYLTYPE *info) {
    YYLTYPE *infoTemp = new YYLTYPE();
    *infoTemp = *info;
    return infoTemp;
}

void printInfo(YYLTYPE *info) {
    std::cout << "Printing YYLTYPE info field, first line: " << info->first_line << " last line: " << info->last_line
    << " first column: " << info->first_column  << " last column " << info->last_column << '\n';
}

/* Check if certain pointer is a null pointer and return true if it is. */
bool check_nullptr(void *vp, std::string type) {
    if(vp == nullptr) {
        std::cout << type << " is nullptr" << std::endl;
        return true;
    }
    return false;
}

/* Check if certain pointer is a nullptr and if that is the case, throws an exception. */
void check_nullptr_throws(void *vp, std::string type) {
    if(vp == nullptr)
        throw type + " is a nullptr exception";
}

void EntryNullPtrMsg(std::string loc, std::string name) {
    std::cout << "error : " + loc + " : no sym table entry obtained from name "
    + name + " (is nullptr).\n";
}

void reverse_vector(std::vector<ASTNode*> &vec) {
    std::reverse(vec.begin(), vec.end());
}

std::string spaced_str(int spaces) {
    return std::string(spaces, ' ');
}

/* Shorthand for couting a string message. */
void cout_str(std::string msg) {
    std::cout << msg << std::endl;
}

/* Check whether a certain file path caints the correct star file extension. */
bool checkCorrectFileExt(std::string fileName) {
    /* If file name has <= 5 characters, it cannot have name and the extension. */
    if(fileName.size() <= 5) {
        return 0;
    }
    return fileName.substr(fileName.size() - 5) == ".star";
}

/* Parsing the given command line arguments. */
compFlags ParseCmmndLineArgs(int argc, char *argv[]) {
    compFlags flags;

    for(int i = 2; i < argc; i++) {
        std::string argvStr = std::string(argv[i]);

        if(argvStr.substr(0,4) == "--d=" && argvStr.size() > 4) {
            flags.customFileDest = 1;
            flags.fileDest = argvStr.substr(4, argvStr.size() - 4);
        }
        else if(argvStr.substr(0,4) == "-ast" && argvStr.size() == 4) {
            flags.printAst = 1;
        }
        else if(argvStr.substr(0,4) == "-tac" && argvStr.size() == 4) {
            flags.printTac = 1;
        }
        else {
            std::cout << "invalid flag : " + argvStr << std::endl;
            flags.invalidFlag = true;
        }
    }

    return flags;
}

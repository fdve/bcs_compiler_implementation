/* File containing utility functions for the compiler. */

/* -[] are all these functions globals, being declared in this way?
  I mean, every file which includes the header has access to the functions
  Not sure if there is really a distiction there between global and not global.
  Defining a variable here would be considered global right, does that mean
  that static variables (and functions?) are kind of global? */

#include <sstream>
#include <string>

#include "../headers/ast.h"
#include "../headers/util.h"
#include "../headers/parser.hpp"


/* Allocate a pointer in the heap and set its contents to given YYLTYPE info struct. */
YYLTYPE* newInfo(YYLTYPE *info) {
    // cout_str("IN NEW INFO, PRINTING INFO :");
    // printInfo(info);



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

/* Shorthand for printing a null terminated string. */
void cout_str(std::string msg) {
    std::cout << msg << std::endl;
}

/* Check whether a certain file path caints the correct star file extension. */
bool checkCorrectFileExt(std::string fileName) {
    return fileName.substr(fileName.size() - 5) == ".star";
}

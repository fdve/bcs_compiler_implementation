/* Initialize the error logger and declare errors messages.
 * Modifications to the error messages can be done in this file,
 * and the test framework will still work, since it these compares error logger
 * template error messages against the same error message template
 * messages which were called by the executing program.
 * - [] expect when we change the arugments for instance? then we still ahve
 * to change in gtest and callers
 */

#include <iostream>
#include <string>
#include <vector>

#include "../headers/ast.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"
#include "../headers/util.h"


void ErrorLogger::ErrorMsg::Print() {
    std::cout << ErrorInfoString(this->info) << " " << this->msg;
}

/* Return a string representing the error message. */
std::string ErrorLogger::ErrorMsg::Str() {
    return ErrorInfoString(this->info) + " " + this->msg;
}

/* Overload == operator to check whether 2 error messages are the same
 * when compared using the == operator. */
bool operator==(ErrorLogger::ErrorMsg lhs, ErrorLogger::ErrorMsg rhs) {
    return lhs.Str() == rhs.Str();
}

/* Overload < operator so that the error messages can be sorted using std::sort. */
bool operator<(ErrorLogger::ErrorMsg lhs, ErrorLogger::ErrorMsg rhs) {
    bool sameLine = lhs.info->first_line == rhs.info->first_line;
    bool lhsLowerColumn = lhs.info->first_column < rhs.info->first_column;
    bool lhsLowerLine = lhs.info->first_line < rhs.info->first_line;

    return sameLine ? lhsLowerColumn : lhsLowerLine;
}

std::vector<ErrorLogger::ErrorMsg> ErrorLogger::errorMsgs;
std::string ErrorLogger::fileName = "unknownFileName";
bool ErrorLogger::parsingError = false;
bool ErrorLogger::trimFilePath = true;
bool ErrorLogger::colorizeOutput = true;

void ErrorLogger::Reset() {
    ErrorLogger::errorMsgs.clear();
    ErrorLogger::parsingError = false;
}

void ErrorLogger::PrintErrors() {
    for(ErrorMsg &error : ErrorLogger::errorMsgs) {
        error.Print();
    }

    if(ErrorLogger::errorMsgs.size() > 0) {
        std::cout << "Total number of errors are " << ErrorLogger::errorMsgs.size() << ".\n";
    }
}

std::string ErrorLogger::FormatType(std::string type) {
    if(type == "")
        return std::string("(") + "unknownType" + ")";
    else
        return "(" + type + ")";
}

std::string ErrorLogger::ErrorInfoString(YYLTYPE *info) {
    return "\033[4;31m" + fileName + ":" + std::to_string(info->first_line) + ":"
    + std::to_string(info->first_column) + ":\033[;31m error\033[0m";
}

YYLTYPE* ErrorLogger::CreateInfo(int firstLine, int firstColumn) {
    YYLTYPE *info = new YYLTYPE();
    info->first_line = firstLine;
    info->first_column = firstColumn;
    return info;
}

/* Since errors can be determined in non sequential order
 * respective to where the errors occurred in the source file,
 * order the error messages on line and column using overloaded operator. */
void ErrorLogger::InsertMsg(ErrorMsg msg) {
    ErrorLogger::errorMsgs.push_back(msg);
    std::sort(ErrorLogger::errorMsgs.begin(), ErrorLogger::errorMsgs.end());
}

/* -[] need to fix yyltype for this one aswell */
ErrorLogger::ErrorMsg ErrorLogger::ParserError(int lineo, std::string errorMsg) {
    return ErrorMsg(CreateInfo(lineo, 0), std::string("line ") + std::to_string(lineo) + ": " + errorMsg + ".\n");

    // return std::string("line ") + std::to_string(lineo) + ": " + errorMsg + ".\n";
}

ErrorLogger::ErrorMsg ErrorLogger::UndefinedFnc(YYLTYPE *info, std::string fnc) {
    return ErrorMsg(info, "function " + fnc + " has not been defined before.\n");
}

ErrorLogger::ErrorMsg ErrorLogger::UndefinedVar(YYLTYPE *info, std::string var) {
    return ErrorMsg(info, "variable " + var + " has not been defined before.\n");
}

ErrorLogger::ErrorMsg ErrorLogger::RedefineVar(YYLTYPE *info, std::string var) {
    return ErrorMsg(info, "variable " + var + " has already been defined in this scope or higher scope,"
    + " shadowing of variables is not allowed.\n");
}

/* -[] fix this one */
ErrorLogger::ErrorMsg ErrorLogger::IllegitimateChar(std::string value) {
    return ErrorMsg(nullptr, "aaa");
    // return "error : value " + value + " is illegitemate char value\n";
}

// ErrorLogger::ErrorMsg ErrorLogger::IncompatibleTypes(std::string type1, std::string type2) {
//     return "error : incompatible types : (" + type1 + ") (" + type2 + ")\n";
// }

ErrorLogger::ErrorMsg ErrorLogger::IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string name, std::string type1, std::string type2) {
    if(nodeName == "return") {
        return ReturnError(info, name, type1, type2);
    }
    else if(nodeName == "variableDef") {
        return ErrorMsg(info, "in variable def, trying to assign value of type "
        + FormatType(type2) + " to " + name + " but variable's declared type is " + FormatType(type1) + ".\n");
    }
    else if(nodeName == "assignment") {
        return ErrorMsg(info, "in assignment, trying to assign value of type "
        + FormatType(type2) + " to " + name + " but variable's declared type is " + FormatType(type1) + ".\n");
    }
    else if(nodeName == "unaryOp") {
        return ErrorMsg(info, "in unary operation, unary op type is "
        + FormatType(type1) + " but type of operand is " + FormatType(type2) + ".\n");
    }
    /* Combine if stmnt and while stmtn? */
    else if(nodeName == "ifStmnt") {
        return ErrorMsg(info, "in If expression, expected type is " + FormatType(type2)
        + " but evaluated type is " + FormatType(type1) + ".\n");
    }
    else if(nodeName == "doWhile" || nodeName == "for" || nodeName == "while") {
        return ErrorMsg(info, "in " + nodeName + " expression, expected type is " + FormatType(type2)
        + " but evaluated type is " + FormatType(type1) + ".\n");
    }
    else {
        return ErrorMsg(info, "" + nodeName + ", type o ("
        + type1 + ") (" + type2 + "), ID: " + name + '\n');
    }
}

ErrorLogger::ErrorMsg ErrorLogger::IncompatibleTypesBinOps(YYLTYPE *info, std::string binop, std::string lType, std::string rType) {
    return  ErrorMsg(info, "in binary operation, type of the left operand " + FormatType(lType)
    + " is incompatible with the right operand type " + FormatType(rType) + ".\n");
}

ErrorLogger::ErrorMsg ErrorLogger::NonVoidFncNoReturn(YYLTYPE *info, std::string funcName) {
    return ErrorMsg(info, "no value returned in non void function " + funcName + '\n');
}

ErrorLogger::ErrorMsg ErrorLogger::ReturnError(YYLTYPE *info, std::string functionName, std::string returnType1, std::string returnType2) {
    return ErrorMsg(info, "in function " + functionName + ", expecting return value of type " + FormatType(returnType1)
    + " but given value is of type " + FormatType(returnType2) + ".\n");
}

ErrorLogger::ErrorMsg ErrorLogger::InvalidNumParams(YYLTYPE *info, std::string functionName, int expected, int given) {
    return ErrorMsg(info, "in function call to " + functionName
    + ", invalid num of params, " + std::to_string(given) + " given but expected " + std::to_string(expected) + ".\n");
}

ErrorLogger::ErrorMsg ErrorLogger::InvalidTypesParams(YYLTYPE *info, std::string functionName, int paramNum, std::string expected, std::string given) {
    return ErrorMsg(info, "mismatch between types of declared function param " + std::to_string(paramNum) + " of "
    + functionName + ", given "+ FormatType(given) + " but expected " + FormatType(expected) + ".\n");
}

ErrorLogger::ErrorMsg ErrorLogger::CalledSymbolIsNotFnc(YYLTYPE *info, std::string functionName) {
    return ErrorMsg(info, "trying to call " + functionName + " which is not a function.\n");
}

ErrorLogger::ErrorMsg ErrorLogger::FncDecTooManyParams(YYLTYPE *info, std::string functionName) {
    return ErrorMsg(info, "declared function " + functionName + " has more than 6 args.\n");
}

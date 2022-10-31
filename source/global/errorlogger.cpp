/* Initialize the error logger and declare errors messages,
 * to be used for testing, and to give feedback to the compiler user
 * about possible errors encountered in the users provided Star source program. */

#include <iostream>
#include <string>
#include <vector>

#include "../headers/ast.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"
#include "../headers/util.h"

void ErrLog::ErrMsg::Print() {
    std::cout << ErrorInfoString(this->info) << " " << this->msg;
}

/* Return a string representing the error message. */
std::string ErrLog::ErrMsg::Str() {
    return ErrorInfoString(this->info) + " " + this->msg;
}

/* Overload == operator to check whether 2 error messages are the same
 * when compared using the == operator. */
bool operator==(ErrLog::ErrMsg lhs, ErrLog::ErrMsg rhs) {
    return lhs.Str() == rhs.Str();
}

/* Overload < operator so that the error messages can be sorted using std::sort. */
bool operator<(ErrLog::ErrMsg lhs, ErrLog::ErrMsg rhs) {
    bool sameLine = lhs.info->first_line == rhs.info->first_line;
    bool lhsLowerColumn = lhs.info->first_column < rhs.info->first_column;
    bool lhsLowerLine = lhs.info->first_line < rhs.info->first_line;

    return sameLine ? lhsLowerColumn : lhsLowerLine;
}

std::vector<ErrLog::ErrMsg> ErrLog::errorMsgs;
std::string ErrLog::fileName = "unknownFileName";
bool ErrLog::parsingError = false;
bool ErrLog::trimFilePath = true;
bool ErrLog::colorizeOutput = true;

void ErrLog::Reset() {
    ErrLog::errorMsgs.clear();
    ErrLog::parsingError = false;
}

void ErrLog::PrintErrors() {
    for(ErrMsg &error : ErrLog::errorMsgs) {
        error.Print();
    }

    if(ErrLog::errorMsgs.size() > 0) {
        std::cout << "Total number of errors are " << ErrLog::errorMsgs.size() << ".\n";
    }
}

std::string ErrLog::FormatType(std::string type) {
    if(type == "")
        return std::string("(") + "unknownType" + ")";
    else
        return "(" + type + ")";
}

std::string ErrLog::ErrorInfoString(YYLTYPE *info) {
    return "\033[4;31m" + fileName + ":" + std::to_string(info->first_line) + ":"
    + std::to_string(info->first_column) + ":\033[;31m error\033[0m";
}

YYLTYPE* ErrLog::CreateInfo(int firstLine, int firstColumn) {
    YYLTYPE *info = new YYLTYPE();
    info->first_line = firstLine;
    info->first_column = firstColumn;
    return info;
}

/* Since errors can be determined in non sequential order
 * in regards to where the errors occurred in the source file,
 * order the error messages on line and column using the overloaded operator. */
void ErrLog::InsertMsg(ErrMsg msg) {
    ErrLog::errorMsgs.push_back(msg);
    std::sort(ErrLog::errorMsgs.begin(), ErrLog::errorMsgs.end());
}

ErrLog::ErrMsg ErrLog::ParserError(int line, int column, std::string errorMsg) {
    return ErrMsg(CreateInfo(line, column), errorMsg + ".\n");
}

ErrLog::ErrMsg ErrLog::UndefinedFnc(YYLTYPE *info, std::string fnc) {
    return ErrMsg(info, "function " + fnc + " has not been defined before.\n");
}

ErrLog::ErrMsg ErrLog::UndefinedVar(YYLTYPE *info, std::string var) {
    return ErrMsg(info, "variable " + var + " has not been defined before.\n");
}

ErrLog::ErrMsg ErrLog::RedefineVar(YYLTYPE *info, std::string var) {
    return ErrMsg(info, "variable " + var + " has already been defined in this scope or higher scope,"
    + " shadowing of variables is not allowed.\n");
}

ErrLog::ErrMsg ErrLog::IllegitimateChar(YYLTYPE *info, std::string value) {
    return ErrMsg(info, "illegitimate char declaration.\n");
}

ErrLog::ErrMsg ErrLog::IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string name, std::string type1, std::string type2) {
    if(nodeName == "return") {
        return ReturnError(info, name, type1, type2);
    }
    else if(nodeName == "variableDef") {
        return ErrMsg(info, "in variable def, trying to assign value of type "
        + FormatType(type2) + " to " + name + " but variable's declared type is " + FormatType(type1) + ".\n");
    }
    else if(nodeName == "assignment") {
        return ErrMsg(info, "in assignment, trying to assign value of type "
        + FormatType(type2) + " to " + name + " but variable's declared type is " + FormatType(type1) + ".\n");
    }
    else if(nodeName == "unaryOp") {
        return ErrMsg(info, "in unary operation, unary op type is "
        + FormatType(type1) + " but type of operand is " + FormatType(type2) + ".\n");
    }
    else if(nodeName == "ifStmnt") {
        return ErrMsg(info, "in If expression, expected type is " + FormatType(type2)
        + " but evaluated type is " + FormatType(type1) + ".\n");
    }
    else if(nodeName == "doWhile" || nodeName == "for" || nodeName == "while") {
        return ErrMsg(info, "in " + nodeName + " expression, expected type is " + FormatType(type2)
        + " but evaluated type is " + FormatType(type1) + ".\n");
    }
    else {
        return ErrMsg(info, "" + nodeName + ", type o ("
        + type1 + ") (" + type2 + "), ID: " + name + '\n');
    }
}

ErrLog::ErrMsg ErrLog::IncompatibleTypesBinOps(YYLTYPE *info, std::string binop, std::string lType, std::string rType) {
    return  ErrMsg(info, "in binary operation " + binop + ", type of the left operand " + FormatType(lType)
    + " is incompatible with the right operand type " + FormatType(rType) + ".\n");
}

ErrLog::ErrMsg ErrLog::NonVoidFncNoReturn(YYLTYPE *info, std::string funcName) {
    return ErrMsg(info, "no value returned in non void function " + funcName + '\n');
}

ErrLog::ErrMsg ErrLog::ReturnError(YYLTYPE *info, std::string functionName, std::string returnType1, std::string returnType2) {
    return ErrMsg(info, "in function " + functionName + ", expecting return value of type " + FormatType(returnType1)
    + " but given value is of type " + FormatType(returnType2) + ".\n");
}

ErrLog::ErrMsg ErrLog::InvalidNumParams(YYLTYPE *info, std::string functionName, int expected, int given) {
    return ErrMsg(info, "in function call to " + functionName
    + ", invalid num of params, " + std::to_string(given) + " given but expected " + std::to_string(expected) + ".\n");
}

ErrLog::ErrMsg ErrLog::InvalidTypesParams(YYLTYPE *info, std::string functionName, int paramNum, std::string expected, std::string given) {
    return ErrMsg(info, "mismatch between types of declared function param " + std::to_string(paramNum) + " of "
    + functionName + ", given "+ FormatType(given) + " but expected " + FormatType(expected) + ".\n");
}

ErrLog::ErrMsg ErrLog::CalledSymbolIsNotFnc(YYLTYPE *info, std::string functionName) {
    return ErrMsg(info, "trying to call " + functionName + " which is not a function.\n");
}

ErrLog::ErrMsg ErrLog::FncDecTooManyParams(YYLTYPE *info, std::string functionName) {
    return ErrMsg(info, "declared function " + functionName + " has more than 6 args.\n");
}

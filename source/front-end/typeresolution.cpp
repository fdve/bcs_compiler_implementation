#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/typeresolution.h"
#include "../headers/errorlogger.h"

/* -[] this gives 0 information about where the incompatible types are */
// void TypeResolution::IncompatibleTypes(std::string type1, std::string type2) {
//     if(type1 != type2) {
//         ErrorLogger::InsertMsg(ErrorLogger::IncompatibleTypes(type1, type2));
//     }
// }

void TypeResolution::IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string name, std::string type1, std::string type2) {
    if(type1 != type2) {
        ErrorLogger::InsertMsg(ErrorLogger::IncompatibleTypes(info, nodeName, name, type1, type2));
    }
}

void TypeResolution::IncompatibleTypesBinOp(YYLTYPE *info, std::string name, Expression *exprLhs, Expression *exprRhs) {
    if(exprLhs->getType() != exprRhs->getType()) {
        ErrorLogger::InsertMsg(ErrorLogger::IncompatibleTypesBinOps(info, name, exprLhs->getType(), exprRhs->getType()));
    }
}

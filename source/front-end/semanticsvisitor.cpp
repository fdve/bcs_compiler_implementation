/* Performs semantic analysis. e.g. check that no more than 6 parameters are given
 * when declaring a function, that binary operations have type comptiable operands, etc. */

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/astvisitor.h"
#include "../headers/semanticsvisitor.h"
#include "../headers/typeresolution.h"
#include "../headers/symboltable.h"
#include "../headers/scope.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"

void SemanticsVisitor::visit(ASTNode *node) {
    check_nullptr_throws(dynamic_cast<Program*>(node), "Program_node");
    visit(dynamic_cast<Program*>(node));
}

void SemanticsVisitor::visit(Program *node) {
    Program *progNode = dynamic_cast<Program*>(node);

    for(FuncDecl *funcDec : *progNode->lhs) {
        visit(funcDec);
    }

    visit(progNode->start_fnc);

    for(FuncDecl *funcDec : *progNode->rhs) {
        visit(funcDec);
    }
}

void SemanticsVisitor::visit(FuncDecl *funcDec) {
    if(funcDec->id == nullptr) {
        return;
    }

    if(funcDec->paramList != nullptr) {
        /* If there are more than 6 parameters, report error since this is not allowed. */
        if(funcDec->paramList->size() > 6) {
            ErrLog::InsertMsg(ErrLog::FncDecTooManyParams(funcDec->info, funcDec->id->name));
        }
    }

    for(Statement *statement : *funcDec->fncBlock) {
        visit(statement);
    }
}

/* Do not have to try to visit do While nodes since do While nodes get desugared into while loops. */
void SemanticsVisitor::visit(Statement *stmnt) {
    if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>(stmnt)) {
        for(Statement *statement : *funcBlock->stmnts) {
            visit(statement);
        }
    }
    else if(VariableDef *varDef = dynamic_cast<VariableDef*>(stmnt)) {
        TypeResolution::annotateExpr(varDef->expr);

        TypeResolution::IncompatibleTypes(varDef->info, "variableDef", varDef->id->name, varDef->type, varDef->expr->getType());
        visit(varDef->expr);
    }
    else if(Assignment *assign = dynamic_cast<Assignment*>(stmnt)) {
        TypeResolution::annotateExpr(assign->expr);

        symTableEntry *entry = assign->id->entryRef;
        if(entry == nullptr) {
            /* If there is no entry, it has never been defined. Return instead.*/
            return;
        }

        TypeResolution::IncompatibleTypes(assign->info, "assignment",  entry->name, entry->type, assign->expr->getType());
        visit(assign->expr);
    }
    else if(ExprStmnt *exprStmnt = dynamic_cast<ExprStmnt*>(stmnt)) {
        visit(exprStmnt->expr);
    }
    else if(ReturnCall *returnCall = dynamic_cast<ReturnCall*>(stmnt)) {
        /* Only need to visit and annotate the return call expression if there is one.*/
        if(returnCall->expr != nullptr) {
            TypeResolution::annotateExpr(returnCall->expr);
            visit(returnCall->expr);
        }

        /* The id of a return call node is the funcs id the call returns to. */
        symTableEntry *entry = returnCall->id->entryRef;
        if(entry == nullptr) {
            EntryNullPtrMsg("SemanticsVisitor : returnCallNode", returnCall->id->name);
            return;
        }

        /* If the return call body expr  is empty, compare the body expr type against void.
           Otherwise, compare it against the type of the body epxr. */
        if(returnCall->expr == nullptr) {
            TypeResolution::IncompatibleTypes(returnCall->info, "return", entry->name, entry->type, "void");
        }
        else {
            TypeResolution::IncompatibleTypes(returnCall->info, "return", entry->name, entry->type, returnCall->expr->getType());
        }
    }
    else if(CFIf *cfIf = dynamic_cast<CFIf*>(stmnt)) {
        TypeResolution::annotateExpr(cfIf->expr);

        /* The expression of the If statement condition needs to be of type bool. */
        TypeResolution::IncompatibleTypes(cfIf->info, "ifStmnt", "?", cfIf->expr->getType(), "bool");

        visit(cfIf->expr);

        if(!check_nullptr(cfIf->ifStmnts, "cfIf->ifStmns")) {
            for(Statement *stmnt : *cfIf->ifStmnts) {
                visit(stmnt);
            }
        }
        if(cfIf->elseStmnts == nullptr) {
            return;
        }
        else {
            for(Statement *stmnt : *cfIf->elseStmnts) {
                visit(stmnt);
            }
        }
    }
    else if(CFWhile *cfWhile = dynamic_cast<CFWhile*>(stmnt)) {
        TypeResolution::annotateExpr(cfWhile->expr);

        std::string str = cfWhile->expr->getType();

        /* If not desugared. */
        if(cfWhile->beforeDesugar == "") {
            TypeResolution::IncompatibleTypes(cfWhile->info, "while", "?", cfWhile->expr->getType(), "bool");
        }
        else {
            TypeResolution::IncompatibleTypes(cfWhile->info, cfWhile->beforeDesugar, "?", cfWhile->expr->getType(), "bool");
        }

        visit(cfWhile->expr);

        for(Statement *stmnt : *cfWhile->stmnts) {
            visit(stmnt);
        }
    }
    else {
        cout_str("error : does not detect a derived class of type stmnt class in type visitor");
    }
}

void SemanticsVisitor::visit(Expression *expr) {
    if(UnaryOpExpr *unaryOp = dynamic_cast<UnaryOpExpr*>(expr)) {
        TypeResolution::annotateExpr(unaryOp);

        TypeResolution::IncompatibleTypes(unaryOp->info, "unaryOp", "?", unaryOp->type, unaryOp->expr->getType());
        visit(unaryOp->expr);
    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        TypeResolution::IncompatibleTypesBinOp(binOpExpr->info, binOpExpr->binOp, binOpExpr->lExpr, binOpExpr->rExpr);
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        TypeResolution::annotateExpr(funcCall);

        symTableEntry *entry = funcCall->id->entryRef;
        if(entry == nullptr) {
            ErrLog::InsertMsg(ErrLog::UndefinedFnc(funcCall->info, funcCall->id->name));
            return;
        }

        if(!Scope::SymbolIsFunc(funcCall->id->name)) {
            ErrLog::InsertMsg(ErrLog::CalledSymbolIsNotFnc(funcCall->info, entry->name));
            return;
        }

        int decParamsSize = (entry->params == nullptr) ? 0 : entry->params->size();
        int calledParamsSize = (funcCall->params == nullptr) ? 0 : funcCall->params->size();

        /* Compare the number of provided args against the number of declared func args. */
        if(decParamsSize != calledParamsSize) {
            ErrLog::InsertMsg(ErrLog::InvalidNumParams(funcCall->info, entry->name, decParamsSize, calledParamsSize));
            return;
        }

        /* Check whether the function args types are the same as the provided arg types. */
        for(int i = 0; i < decParamsSize; i++) {
            std::string funcDecParamType = entry->params->at(i)->type;
            std::string funcCallParamType = funcCall->params->at(i)->getType();

            if(funcDecParamType != funcCallParamType) {
                ErrLog::InsertMsg(ErrLog::InvalidTypesParams(funcCall->info, entry->name, i+1, funcDecParamType, funcCallParamType));
            }
        }
    }
    else if(expr == nullptr) {
        cout_str("error : printvisitor, visit, expr is nullptr");
    }
}

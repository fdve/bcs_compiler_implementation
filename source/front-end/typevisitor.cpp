/* Checks type compatibility between bin / rel ops, function calls and
   their params, return call and the function expressions, and so forth.

   The AST nodes get annotated with the correct type on creation.
*/

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/astvisitors.h"
#include "../headers/typeresolution.h"
#include "../headers/symboltable.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"

void TypeVisitor::visit(ASTNode *node) {
    check_nullptr_throws(dynamic_cast<Program*>(node), "Program_node");
    visit(dynamic_cast<Program*>(node));
}

void TypeVisitor::visit(Program *node) {
    Program *progNode = dynamic_cast<Program*>(node);

    for(FuncDecl *funcDec : *progNode->lhs) {
        visit(funcDec);
    }

    visit(progNode->start_fnc);

    for(FuncDecl *funcDec : *progNode->rhs) {
        visit(funcDec);
    }
}

void TypeVisitor::visit(FuncDecl *funcDec) {
    if(funcDec->id == nullptr) {
        return;
    }

    if(funcDec->paramList != nullptr) {
        /* If there are more than 6 parameters, report error since this is not allowed. */
        if(funcDec->paramList->size() > 6) {
            ErrorLogger::InsertMsg(ErrorLogger::FncDecTooManyParams(funcDec->info, funcDec->id->name));
        }

        for(FuncDeclParam *funcDeclParam : *funcDec->paramList) {
            visit(funcDeclParam);
        }
    }

    for(Statement *statement : *funcDec->fncBlock) {
        visit(statement);
    }

    /* - [] disable return checking in non void function for now*/
    // bool noReturnCall = true;
    // for(Statement *statement : *funcDec->fncBlock) {
    //     visit(statement);
    //
    //     if(dynamic_cast<ReturnCall*>(statement) != nullptr)
    //         noReturnCall = false;
    // }
    //
    // /* If the return type is different than void, and no return call node was found, output error. */
    // if(noReturnCall && funcDec->id->type != "void") {
    //     ErrorLogger::InsertMsg(ErrorLogger::NonVoidFncNoReturn(funcDec->info, (funcDec->id->name)));
    // }
}

void TypeVisitor::visit(FuncDeclParam *funcDeclParam) {
    // std::cout << "FncDecParam type " << funcDeclParam->type << " id " << funcDeclParam->id->name << std::endl;
}

void TypeVisitor::visit(Statement *stmnt) {
    /* Check number of */

    if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>(stmnt)) {
        for(Statement *statement : *funcBlock->stmnts) {
            visit(statement);
        }
    }
    else if(VariableDef *varDef = dynamic_cast<VariableDef*>(stmnt)) {
        annotateExpr(varDef->expr);

        TypeResolution::IncompatibleTypes(varDef->info, "variableDef", varDef->id->name, varDef->type, varDef->expr->getType());
        visit(varDef->expr);
    }
    else if(Assignment *assign = dynamic_cast<Assignment*>(stmnt)) {
        annotateExpr(assign->expr);

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
        annotateExpr(returnCall->expr);
        visit(returnCall->expr);



        /* The latest added function is the function which is being returned to. */
        // const FuncDecl* funcDeclNode = this->funcDeclNodes.at(funcDeclNodes.size() - 1);

        // Scope::PrintScopes(Scope::globalScope, 0);

        /* Gives null ptr since all the scopes are popped at this point, and
           the scopes are traversed to look for entries. */
        // symTableEntry *entry1 = Scope::GetSymbol("start");
        // if(entry1 == nullptr) cout_str("nullptrrrrrrrrrrrrrrrrr");


        /* The id of a return call node is the funcs id the call returns to. */
        symTableEntry *entry = returnCall->id->entryRef;
        if(entry == nullptr) {
            EntryNullPtrMsg("TypeVisitor : returnCallNode", returnCall->id->name);
            return;
        }


        // cout_str("RETURN CALL ID NAME = " + returnCall->id->name);
        // printInfo(returnCall->info);


        /* this crashes */
        // returnCall->expr->getType();

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
        annotateExpr(cfIf->expr);

        // std::cout << "IfStmnt" << std::endl;

        /* The expression of the If statement header needs to be of type bool. */
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
        // if(!check_nullptr(cfIf->elseStmnts, "cfIf->elseStmnts")){
        // }
    }
    else if(CFWhile *cfWhile = dynamic_cast<CFWhile*>(stmnt)) {
        // std::cout << "WhileStmnt" << std::endl;

        annotateExpr(cfWhile->expr);


        // YYLTYPE *inf = cfIf->info;

        std::string str = cfWhile->expr->getType();

        /* If not desugared */
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
    else if(CFDoWhile *cfDoWhile = dynamic_cast<CFDoWhile*>(stmnt)) {
        // cout_str("DO WHILE IN STMNT TYPE VISITOR, WE DO NOT COME HERE DO WE? SINCE WE DESUGAR OUR DOWHILE NODES.");
        // std::cout << "DoWhileStmnt" << std::endl;
        visit(cfDoWhile->expr);

        for(Statement *stmnt : *cfDoWhile->stmnts) {
            visit(stmnt);
        }
    }
    else {
        cout_str("error : does not detect a derived class of type stmnt class in type visitor");
    }
}

void TypeVisitor::visit(Expression *expr) {
    if(UnaryOpExpr *unaryOp = dynamic_cast<UnaryOpExpr*>(expr)) {
        // std::cout << typeid(unaryOp->expr).name() << std::endl;
        annotateExpr(unaryOp);

        TypeResolution::IncompatibleTypes(unaryOp->info, "unaryOp", "??????"
        , unaryOp->type, unaryOp->expr->getType());
        visit(unaryOp->expr);
    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        TypeResolution::IncompatibleTypesBinOp(binOpExpr->info, "bin expr can have non nameable lhs and rhs?", binOpExpr->lExpr, binOpExpr->rExpr);



        // visit(binOpExpr->lExpr);
        // std::cout << "binOpExpr bin_op:" << binOpExpr->bin_op << std::endl;
        // visit(binOpExpr->rExpr);
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        annotateExpr(funcCall);

        /* (1) Check if the id of function being called is legitimate (could be a var). */

        // cout_str(funcCall->id->name);
        // const FuncDecl *funcDec = this->GetFuncDecl(funcCall->id->name);

        symTableEntry *entry = funcCall->id->entryRef;
        if(entry == nullptr) {
            // EntryNullPtrMsg("TypeVisitor : funcCallNode", funcCall->id->name);
            ErrorLogger::InsertMsg(ErrorLogger::UndefinedFnc(funcCall->info, funcCall->id->name));
            return;
        }
        // cout_str("TypeVisitor : funcCallNode, funcCall->id->name : " + funcCall->id->name);

        /* NEED TO CHECK THAT GIVEN ENTRY IS REALLY A FUNCVTION
        IF NOT IN GLOBAL SCOPE, NOT FUNCTION !
        */
        if(!Scope::SymbolIsFunc(funcCall->id->name)) {
            ErrorLogger::InsertMsg(ErrorLogger::CalledSymbolIsNotFnc(funcCall->info, entry->name));
            return;
        }

        int decParamsSize = (entry->params == nullptr) ? 0 : entry->params->size();
        int calledParamsSize = (funcCall->params == nullptr) ? 0 : funcCall->params->size();

        cout_str("decParamsSize = " + std::to_string(decParamsSize));
        cout_str("calledParamsSize = " + std::to_string(calledParamsSize));


        /* Compare the number of provided args against the number of declared func args. */
        // if(funcDec->paramList->size() != funcCall->params->size()) { // - [] old one
        if(decParamsSize != calledParamsSize) {
            //throw ("functionCallError : number of args provided for function " + funcDec->id->name + " is incorrect.");

            /* -[] shouldnt this output to the error logger? */
            // cout_str("error : number of args provided for function " + entry->name + " is incorrect.");
            ErrorLogger::InsertMsg(ErrorLogger::InvalidNumParams(funcCall->info, entry->name, decParamsSize, calledParamsSize));
            return;
        }

        /* Check whether the function args types are compatible with the provided arg types. */
        for(int i = 0; i < decParamsSize; i++) {
            std::string funcDecParamType = entry->params->at(i)->type;
            std::string funcCallParamType = funcCall->params->at(i)->getType();

            // cout_str(entry->name + " " + funcDecParamType);
            // cout_str(funcCall->id->name + " " + funcCallParamType);

            if(funcDecParamType != funcCallParamType) {
                /* -[] output to error logger? */

                ErrorLogger::InsertMsg(ErrorLogger::InvalidTypesParams(funcCall->info, entry->name, i+1, funcDecParamType, funcCallParamType));


                // std::string excep_str = std::string("error : mismatch between types of the declared funtion param and provided function params, types ")
                // + "(" + funcDecParamType + ") (" + funcCallParamType + ")";
                // throw excep_str;
                // cout_str(excep_str);
            }
        }

        // std::cout << "Funccall id:" << funcCall->id->name << std::endl;
        // if(check_nullptr(funcCall->params, "funcCall params")) {
        //     return;
        // }
        //
        // for(Expression *expr : *funcCall->params) {
        //     visit(expr);
        // }
    }

    // else if(Identifier *identifier = dynamic_cast<Identifier*>(expr)) {
    //     // std::cout << "Identifier name:" << identifier->name << std::endl;
    // }
    // else if(IntConst *constant = dynamic_cast<IntConst*>(expr)) {
    //     // std::cout << "IntConst value:" << constant->value << std::endl;
    // }
    // else if(FloatConst *constant = dynamic_cast<FloatConst*>(expr)) {
    //     // std::cout << "FloatConst value:" << constant->value << std::endl;
    // }
    // else if(BoolConst *constant = dynamic_cast<BoolConst*>(expr)) {
    //     // std::cout << "BoolConst value:" << constant->value << std::endl;
    // }
    // else if(CharConst *constant = dynamic_cast<CharConst*>(expr)) {
    //     // std::cout << "CharConst value:" << constant->value << " type: " << constant->type << std::endl;
    // }
    else if(expr == nullptr) {
        cout_str("error : printvisitor, visit, expr is nullptr");
    }
    // else {
    //     cout_str("error : does not detect a derived class of type expr class in type visitor");
    // }
}

/* - [] move towards different file? */
/* Annotate untyped ID's found within expressions with their correct type. */
void TypeVisitor::annotateExpr(Expression *expr) {
    // cout_str("in annotate expr");

    if(UnaryOpExpr *unaryOpExpr = dynamic_cast<UnaryOpExpr*>(expr)) {
        annotateExpr(unaryOpExpr->expr);
        annotateUnaryOp(unaryOpExpr);

    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        annotateExpr(binOpExpr->lExpr);
        annotateExpr(binOpExpr->rExpr);
        annotateBinOp(binOpExpr);
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        // if(check_nullptr(funcCall->params, "funcCall params")) {
        if(funcCall->params == nullptr) {
            return;
        }

        for(Expression *funcCallArg : *funcCall->params) {
            annotateExpr(funcCallArg);
        }
    }
    else if(Identifier *identifier = dynamic_cast<Identifier*>(expr)) {
        if(identifier->getType() != "") {
            return;
        }

        symTableEntry *entry = Scope::GetSymbol(identifier->name);

        if(entry == nullptr) {
            // -[] summarize this comment.
            /* Every parsed identifier gets annotated with its declaration info.
             * However, some identifiers can be generated in desugring process,
             * e.g. a += 2 -> a = a + b. The a present in the rhs will have no
             * source info present with it. If there is no entry (undefined var),
             * and it is generated like this having no nullptr, return immediately.
             * Error message has already been generated by the frist encountering
             * of the lvalue encounterting of a. */
            if(identifier->info == nullptr) {
                return;
            }
            // EntryNullPtrMsg("annotateExpr(), identifierNode", identifier->name);
            ErrorLogger::InsertMsg(ErrorLogger::UndefinedVar(identifier->info, identifier->name));
            return;
        }

        identifier->type = entry->type;
    }
    else if(expr == nullptr) {
        // cout_str("error : typevisitor, in annotateExpr, visit, expr is nullptr");
    }
}

void TypeVisitor::annotateBinOp(BitwiseOpExpr *node) {
    std::vector<std::string> arithmeticOps = { "+", "-", "*", "/", "%" };
    std::vector<std::string> boolOps = {"<", ">", "<=", ">=", "!=", "==", "||", "&&"};

    // cout_str("node->bin_op = " + node->bin_op);

    /* DOESNT WORK IF L EXPR HAS VARS FOR INSTANCE -.-
        -> now should work because we annotate the sub expressions
        before we anotate the bin op expression?
        */
    for(const std::string &op : arithmeticOps) {
        if(node->bin_op == op) {
            node->type = node->lExpr->getType();
            return;
        }
    }

    for(const std::string &op : boolOps) {
        if(node->bin_op == op) {
            node->type = "bool";
            return;
        }
    }

    cout_str("error : typeerror :  could not find given binary op " + node->bin_op);
}

void TypeVisitor::annotateUnaryOp(UnaryOpExpr *node) {
    if(node->unaryOp == '!') {
        node->type = "bool";
    }
    else if(node->unaryOp == '-') {
        std::string exprType = node->expr->getType();

        if(exprType == "char") {
            // throw "invalid type of UnaryOpExpr";


            cout_str("error : unary operator expression is of type (char). Only int, float and bool are allowed.");
        }

        node->type = exprType;
    }
    else {
        cout_str("error : unary op expr constructor encountering unary op != '!' || '-'");
    }
}

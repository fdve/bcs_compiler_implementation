#include "../headers/util.h"
#include "../headers/symboltable.h"
#include "../headers/ast.h"
#include "../headers/astvisitors.h"

/* A basic visitor which traverses the AST, and depending on what kind of node
 * it visits, it will print information about that node.
 * Mainly used for debugging purposes. */

/* The amount of extra spaces the visitor will print depending on current tree height. */
#define DEPTH_INCR 3

void astvisitor_error(std::string nodetype) {
    std::cout << "error : incorrect node type for " << nodetype << " in PrintVisitor" << std::endl;
}

void PrintVisitor::visit(ASTNode *node, int depth) {
    if(dynamic_cast<Program*>(node)) {
        visit(dynamic_cast<Program*>(node), 0);
    }
    else {
        astvisitor_error("ASTNode");
    }
}

/* Print visitor implementation. */
void PrintVisitor::visit(Program *node, int depth) {
    Program *progNode = dynamic_cast<Program*>(node);

    for(FuncDecl *funcDec : *progNode->lhs) { visit(funcDec, DEPTH_INCR); }
    visit(progNode->start_fnc, DEPTH_INCR);
    for(FuncDecl *funcDec : *progNode->rhs) { visit(funcDec, DEPTH_INCR); }
}

void PrintVisitor::visit(FuncDecl *funcDec, int depth) {
    if(!funcDec->id) {
        std::cout << spaced_str(depth) << "FuncDecl = id is nullptr, return." << std::endl;
        return;
    }

    std::cout << spaced_str(depth) << "FuncDec type:" << funcDec->type << " id:" << funcDec->id->name;

    if(funcDec->id->entryRef == nullptr) {
        std::cout << spaced_str(depth) << "symTableEntry: nullptr" << std::endl;
    }
    else {
        std::cout << "  " << "symTableEntry : ";
        std::cout << "EntryAddress:  " << &(*(funcDec->id->entryRef));
        SymbolTable::PrintEntry(*(funcDec->id->entryRef), 1);
    }

    if(!funcDec->paramList) {
        std::cout << spaced_str(depth) << "paramList is nullptr of func decl  id:" << funcDec->id->name << std::endl;
    }
    else {
        for(FuncDeclParam *funcDeclParam : *funcDec->paramList) {
            visit(funcDeclParam, depth + DEPTH_INCR);
        }
    }

    for(Statement *statement : *funcDec->fncBlock) {
        visit(statement, depth + DEPTH_INCR);
    }
}

void PrintVisitor::visit(FuncDeclParam *funcDeclParam, int depth) {
    std::cout << spaced_str(depth) << "FncDecParam type " << funcDeclParam->type << " id " << funcDeclParam->id->name << std::endl;
}

void PrintVisitor::visit(Statement *stmnt, int depth) {
    if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>(stmnt)) {
        for(Statement *statement : *funcBlock->stmnts) {
            visit(statement, depth + DEPTH_INCR);
        }
    }
    else if(VariableDef *temp = dynamic_cast<VariableDef*>(stmnt)) {
        std::cout << spaced_str(depth) << "VarDef type:" << temp->type << " id:" << temp->id->name;

        if(temp->id->entryRef == nullptr) {
            std::cout << spaced_str(depth) << "symTableEntry: nullptr" << std::endl;
        }
        else {
            std::cout << "  " << "symTableEntry : ";
            std::cout << "EntryAddress:  " << &(*(temp->id->entryRef));
            SymbolTable::PrintEntry(*(temp->id->entryRef), 1);
        }

        visit(temp->expr, depth + DEPTH_INCR);
    }
    else if(Assignment *assign = dynamic_cast<Assignment*>(stmnt)) {
        std::cout << spaced_str(depth) << "Assignment id:" << assign->id->name << " op:" << assign->assignOp;

        if(assign->id->entryRef == nullptr) {
            std::cout << spaced_str(depth) << "symTableEntry: nullptr" << std::endl;
        }
        else {
            std::cout << "  " << "symTableEntry : ";
            std::cout << "EntryAddress:  " << &(*(assign->id->entryRef));
            SymbolTable::PrintEntry(*(assign->id->entryRef), 1);
        }

        visit(assign->expr, depth + DEPTH_INCR);
    }
    else if(ExprStmnt *exprStmnt = dynamic_cast<ExprStmnt*>(stmnt)) {
        std::cout << spaced_str(depth) << "exprStmnt" << std::endl;
        visit(exprStmnt->expr, depth + DEPTH_INCR);
    }
    else if(ReturnCall *returnCall = dynamic_cast<ReturnCall*>(stmnt)) {
        if(returnCall->expr == nullptr) {
            std::cout << spaced_str(depth) << "ReturnCall statement : empty" << std::endl;
        }
        else {
            std::cout << spaced_str(depth) << "ReturnCall statement : not empty" << std::endl;
            visit(returnCall->expr, depth + DEPTH_INCR);
        }
    }
    else if(CFIf *cfIf = dynamic_cast<CFIf*>(stmnt)) {
        std::cout << spaced_str(depth) << "IfStmnt" << std::endl;
        visit(cfIf->expr, depth = DEPTH_INCR);
        if(!check_nullptr(cfIf->ifStmnts, "cfIf->ifStmns")) {
            for(Statement *stmnt : *cfIf->ifStmnts) {
                visit(stmnt, depth + DEPTH_INCR);
            }
        }
        if(!check_nullptr(cfIf->elseStmnts, "cfIf->elseStmnts")){
            for(Statement *stmnt : *cfIf->elseStmnts) {
                visit(stmnt, depth + DEPTH_INCR);
            }
        }
    }
    else if(CFFor *cfFor = dynamic_cast<CFFor*>(stmnt)) {
        std::cout << spaced_str(depth) << "ForStmnt" << std::endl;
        if(!check_nullptr(cfFor->varDef, "cfFor varDef")) { visit(cfFor->varDef, depth + DEPTH_INCR); }
        if(!check_nullptr(cfFor->m_expr, "cfFor m_expr")) { visit(cfFor->m_expr, depth + DEPTH_INCR); }
        if(!check_nullptr(cfFor->assign, "cfFor assign")) { visit(cfFor->assign, depth + DEPTH_INCR); }

        if(!check_nullptr(cfFor->stmnts, "cfFor->stmnts")){
            std::cout << spaced_str(depth) << "For loop body stmnts:" << std::endl;
            for(Statement *stmnt : *cfFor->stmnts) {
                visit(stmnt, depth + DEPTH_INCR);
            }
        }
    }
    else if(CFWhile *cfWhile = dynamic_cast<CFWhile*>(stmnt)) {
        std::cout << spaced_str(depth) << "WhileStmnt" << std::endl;
        visit(cfWhile->expr, depth + DEPTH_INCR);

        for(Statement *stmnt : *cfWhile->stmnts) {
            visit(stmnt, depth + DEPTH_INCR);
        }
    }
    else if(CFDoWhile *cfDoWhile = dynamic_cast<CFDoWhile*>(stmnt)) {
        std::cout << spaced_str(depth) << "DoWhileStmnt" << std::endl;
        visit(cfDoWhile->expr, depth + DEPTH_INCR);

        for(Statement *stmnt : *cfDoWhile->stmnts) {
            visit(stmnt, depth + DEPTH_INCR);
        }
    }
    else {
        astvisitor_error("stmnt");
    }
}

void PrintVisitor::visit(Expression *expr, int depth) {
    std::cout << spaced_str(depth) << "EXPR TYPE :" << expr->getType() << std::endl;

    if(UnaryOpExpr *unaryOpExpr = dynamic_cast<UnaryOpExpr*>(expr)) {
        std::cout << spaced_str(depth) << "unaryOpExpr un_op:" << unaryOpExpr->unaryOp << std::endl;
        visit(unaryOpExpr->expr, depth + DEPTH_INCR);
    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        visit(binOpExpr->lExpr, depth + DEPTH_INCR);
        std::cout << spaced_str(depth) << "binOpExpr bin_op:" << binOpExpr->bin_op << std::endl;
        visit(binOpExpr->rExpr, depth + DEPTH_INCR);
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        std::cout << spaced_str(depth) << "Funccall id:" << funcCall->id->name << std::endl;
        if(check_nullptr(funcCall->params, "funcCall params"))
            return;

        for(Expression *expr : *funcCall->params) {
            visit(expr, depth + DEPTH_INCR);
        }
    }
    else if(Identifier *identifier = dynamic_cast<Identifier*>(expr)) {
        std::cout << spaced_str(depth) << "Identifier name:" << identifier->name << " ";

        if(identifier->entryRef == nullptr) {
            std::cout << spaced_str(depth) << "symTableEntry: nullptr" << std::endl;
        }
        else {
            std::cout << "  " << "symTableEntry : ";
            std::cout << "EntryAddress:  " << &(*(identifier->entryRef));
            SymbolTable::PrintEntry(*(identifier->entryRef), 1);
        }
    }
    else if(IntConst *constant = dynamic_cast<IntConst*>(expr)) {
        std::cout << spaced_str(depth) << "IntConst value:" << constant->value << std::endl;
    }
    else if(FloatConst *constant = dynamic_cast<FloatConst*>(expr)) {
        std::cout << spaced_str(depth) << "FloatConst value:" << constant->value << std::endl;
    }
    else if(BoolConst *constant = dynamic_cast<BoolConst*>(expr)) {
        std::cout << spaced_str(depth) << "BoolConst value:" << constant->value << std::endl;
    }
    else if(CharConst *constant = dynamic_cast<CharConst*>(expr)) {
        std::cout << spaced_str(depth) << "CharConst value:" << constant->value << " type: " << constant->type << std::endl;
    }
    else if(expr == nullptr) {
        cout_str("error : printvisitor, visit, expr is nullptr");
    }
    else {
        astvisitor_error("expr");
    }
}

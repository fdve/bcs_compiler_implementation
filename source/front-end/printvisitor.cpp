/* A basic visitor which traverses the AST, and depending on what kind of node
* it visits, it will print information about that node.
* Uses PrettyTree class for pretty printing the AST. */

#include <utility>

#include "../headers/util.h"
#include "../headers/symboltable.h"
#include "../headers/ast.h"
#include "../headers/astvisitor.h"
#include "../headers/printvisitor.h"
#include "../headers/prettytree.h"


void PrintVisitorError(std::string nodetype) {
    std::cout << "error : incorrect node type for " << nodetype << " in PrintVisitor" << std::endl;
}

void PrintVisitor::visit(ASTNode *node) {
    if(dynamic_cast<Program*>(node)) {
        visit(dynamic_cast<Program*>(node));
    }
    else {
        PrintVisitorError("ASTNode");
    }
}

/* Print visitor implementation. */
void PrintVisitor::visit(Program *node) {
    /* Initialize PrettyTree. */
    PrettyTree::InitPrettyTree();

    Program *progNode = dynamic_cast<Program*>(node);
    std::cout << "Program" << std::endl;

    for(FuncDecl *funcDec : *progNode->lhs) {
        visit(funcDec);
    }

    PrettyTree::lastFuncDec = progNode->rhs->size() != 0 ? false : true;
    visit(progNode->start_fnc);

    for(FuncDecl *funcDec : *progNode->rhs) {
        if(funcDec == progNode->rhs->back()) {
            PrettyTree::lastFuncDec = true;
            visit(funcDec);
        }
        else {
            visit(funcDec);
        }
    }
}

void PrintVisitor::visit(FuncDecl *funcDec) {
    if(!funcDec->id) {
        std::cout << "FuncDecl id is nullptr, return." << std::endl;
        return;
    }

    std::cout << PrettyTree::FncDecBranches() << "FuncDec returnT:" << funcDec->type << " id:" << funcDec->id->name;

    if(funcDec->paramList) {
        std::cout << " Params: ";
        for(FuncDeclParam *funcDeclParam : *funcDec->paramList) {
            std::cout << funcDeclParam->type << " " << funcDeclParam->id->name;
            if(funcDeclParam != funcDec->paramList->back()) {
                std::cout << ", ";
            }
        }
    }
    std::cout << std::endl;

    for(Statement *statement : *funcDec->fncBlock) {
        PrettyTree::lastRootStmnt = statement == funcDec->fncBlock->back() ? true : false;
        PrettyTree::lastNestedStmnts.clear();
        visit(statement);
    }
}

/* Deal with nested statements */
void PrintVisitor::visit(Statement *stmnt) {
    if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>(stmnt)) {
        for(Statement *statement : *funcBlock->stmnts) {
            visit(statement);
        }
    }
    else if(VariableDef *temp = dynamic_cast<VariableDef*>(stmnt)) {
        std::cout << PrettyTree::AllStmntBranches() << "VarDef declT:" << temp->type << " id:" << temp->id->name << std::endl;
        visit(temp->expr);
    }
    else if(Assignment *assign = dynamic_cast<Assignment*>(stmnt)) {
        std::cout << PrettyTree::AllStmntBranches() <<  "Assignment id:" << assign->id->name << " op:" << assign->assignOp << std::endl;
        visit(assign->expr);
    }
    else if(ExprStmnt *exprStmnt = dynamic_cast<ExprStmnt*>(stmnt)) {
        std::cout << PrettyTree::AllStmntBranches() << "ExprStmnt" << std::endl;
        visit(exprStmnt->expr);
    }
    else if(ReturnCall *returnCall = dynamic_cast<ReturnCall*>(stmnt)) {
        std::cout << PrettyTree::AllStmntBranches() << "ReturnCall" << std::endl;
        if(returnCall->expr) {
            visit(returnCall->expr);
        }
    }
    else if(CFIf *cfIf = dynamic_cast<CFIf*>(stmnt)) {
        std::cout << PrettyTree::AllStmntBranches() << "If" << std::endl;

        bool noIfStmnts = !cfIf->ifStmnts || cfIf->ifStmnts->size() == 0 ? true : false;
        bool noElseStmnts = !cfIf->elseStmnts || cfIf->elseStmnts->size() == 0 ? true : false;
        PrettyTree::noStmntsCF = noIfStmnts && noElseStmnts ? true : false;

        PrettyTree::visitCFCond = true;
        visit(cfIf->expr);
        PrettyTree::visitCFCond = false;

        if(!check_nullptr(cfIf->ifStmnts, "cfIf->ifStmns")) {
            PrettyTree::lastNestedStmnts.push_back(false);

            for(Statement *stmnt : *cfIf->ifStmnts) {
                PrettyTree::lastDepthStmnt = stmnt == cfIf->ifStmnts->back() && noElseStmnts? true : false;
                PrettyTree::lastNestedStmnts.at(PrettyTree::lastNestedStmnts.size() - 1) = PrettyTree::lastDepthStmnt;
                visit(stmnt);
            }
            PrettyTree::lastNestedStmnts.pop_back();
        }

        if(cfIf->elseStmnts != nullptr){
            PrettyTree::lastNestedStmnts.push_back(false);

            for(Statement *stmnt : *cfIf->elseStmnts) {
                PrettyTree::lastDepthStmnt = stmnt == cfIf->elseStmnts->back() ? true : false;
                PrettyTree::lastNestedStmnts.at(PrettyTree::lastNestedStmnts.size() - 1) = PrettyTree::lastDepthStmnt;
                visit(stmnt);
            }
            PrettyTree::lastNestedStmnts.pop_back();
        }
    }
    else if(CFFor *cfFor = dynamic_cast<CFFor*>(stmnt)) {
        /* Since we now print the AST after the semantic analyis,
         * this code has become redundant and outdated, since all for statements
         * are desugared into while statements, as well as that this code
         * does not employ the new pretty print but employs the old printing.  */

        std::cout << PrettyTree::AllStmntBranches() << "For" << std::endl;

        if(!check_nullptr(cfFor->varDef, "cfFor varDef")) { visit(cfFor->varDef); }
        if(!check_nullptr(cfFor->m_expr, "cfFor m_expr")) { visit(cfFor->m_expr); }
        if(!check_nullptr(cfFor->assign, "cfFor assign")) { visit(cfFor->assign); }

        if(!check_nullptr(cfFor->stmnts, "cfFor->stmnts")){
            for(Statement *stmnt : *cfFor->stmnts) {
                visit(stmnt);
            }
        }
    }
    else if(CFWhile *cfWhile = dynamic_cast<CFWhile*>(stmnt)) {
        std::cout << PrettyTree::AllStmntBranches() << "While" << std::endl;

        bool noStatements = !cfWhile->stmnts || cfWhile->stmnts->size() == 0 ? true : false;
        PrettyTree::noStmntsCF = noStatements ? true : false;

        PrettyTree::visitCFCond = true;
        visit(cfWhile->expr);
        PrettyTree::visitCFCond = false;

        PrettyTree::lastNestedStmnts.push_back(false);

        for(Statement *stmnt : *cfWhile->stmnts) {
            PrettyTree::lastDepthStmnt = stmnt == cfWhile->stmnts->back() ? true : false;
            PrettyTree::lastNestedStmnts.at(PrettyTree::lastNestedStmnts.size() - 1) = PrettyTree::lastDepthStmnt;
            visit(stmnt);
        }
        PrettyTree::lastNestedStmnts.pop_back();
    }
    else if(CFDoWhile *cfDoWhile = dynamic_cast<CFDoWhile*>(stmnt)) {
        /* Since we now print the AST after the semantic analyis,
         * this code has become redundant and outdated, since all do while statements
         * are desugared into while statements, as well as that this code
         * does not employ the new pretty print but employs the old printing.  */

        std::cout << PrettyTree::AllStmntBranches() << "DoWhile" << std::endl;
        visit(cfDoWhile->expr);

        for(Statement *stmnt : *cfDoWhile->stmnts) {
            visit(stmnt);
        }
    }
    else {
        PrintVisitorError("stmnt");
    }
}


void PrintVisitor::visit(Expression *expr) {
    /* Output the type of the expression. */
    std::string t = " t:";
    if(expr != nullptr) {
        t += expr->getType();
    }

    if(UnaryOpExpr *unaryOpExpr = dynamic_cast<UnaryOpExpr*>(expr)) {
        std::cout << PrettyTree::AllExprBranches(expr) << "UnaryOp op:" << unaryOpExpr->unaryOp << t << std::endl;

        PrettyTree::currExprRSubtree.push_back(false);
        visit(unaryOpExpr->expr);
        PrettyTree::currExprRSubtree.pop_back();
    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        PrettyTree::currExprRSubtree.push_back(true);
        visit(binOpExpr->rExpr);
        PrettyTree::currExprRSubtree.pop_back();

        std::cout << PrettyTree::AllExprBranches(expr) << "BinOp op:" << binOpExpr->binOp << t << std::endl;

        PrettyTree::currExprRSubtree.push_back(false);
        visit(binOpExpr->lExpr);
        PrettyTree::currExprRSubtree.pop_back();
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        std::cout << PrettyTree::AllExprBranches(expr) << "FuncCall id:" << funcCall->id->name << t << std::endl;
        if(funcCall->params != nullptr) {
            for(Expression *expr : *funcCall->params) {
                PrettyTree::lastFncCallArg = expr == funcCall->params->back();
                PrettyTree::inFncCallArgExpr = true;

                PrettyTree::currExprRSubtree.push_back(false);
                visit(expr);
                PrettyTree::currExprRSubtree.pop_back();
            }
            PrettyTree::lastFncCallArg = false;
            PrettyTree::inFncCallArgExpr = false;
        }
    }
    else if(Identifier *identifier = dynamic_cast<Identifier*>(expr)) {
        std::cout <<  PrettyTree::AllExprBranches(expr) << "Identifier name:" << identifier->name << t << std::endl;
    }
    else if(IntConst *constant = dynamic_cast<IntConst*>(expr)) {
        std::cout <<  PrettyTree::AllExprBranches(expr) << "IntConst val:" << constant->value << t << std::endl;
    }
    else if(FloatConst *constant = dynamic_cast<FloatConst*>(expr)) {
        std::cout << PrettyTree::AllExprBranches(expr) << "FloatConst val:" << constant->value << t << std::endl;
    }
    else if(BoolConst *constant = dynamic_cast<BoolConst*>(expr)) {
        std::cout << PrettyTree::AllExprBranches(expr) << "BoolConst val:" << constant->value << t << std::endl;
    }
    else if(CharConst *constant = dynamic_cast<CharConst*>(expr)) {
        std::cout << PrettyTree::AllExprBranches(expr) << "CharConst val:" << constant->value << t << std::endl;
    }
    else if(expr == nullptr) {
        cout_str("error : printvisitor, visit, expr is nullptr");
    }
    else {
        PrintVisitorError("expr");
    }
}

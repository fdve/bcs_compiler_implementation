/* Class which desugars statements like k -= 1; to k = k - 1; and
 * translates for loops and do while loops to while loops. */

#include <unordered_map>

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/desugar.h"
#include "../headers/astvisitors.h"

const std::unordered_map<std::string, int> modifiers {
     {"+=", 1},
     {"-=", 2},
     {"*=", 3},
     {"/=", 4},
     {"%=", 5}
 };

int findModifier(std::string mod) {
    auto search = modifiers.find(mod);

    if (search != modifiers.end()) {
        return search->second;
    }
    else {
        return 0;
    }
}

/* -[] gtest bug, prolly need to give proper yyltype info to lexpr */
void DesugarAssignModifier(Assignment *assignNode, Expression *lExpr, std::string op, Expression *rExpr, YYLTYPE *info ) {
    BitwiseOpExpr *binOpExpr = new BitwiseOpExpr(lExpr, std::string(1, op.at(0)), rExpr, info);

    /* -[] should this be static? */

    /* If the identifier of the newly created bin operation is undefined,
     * do not try to annotate it with a type, otherwise when annotating the
     * same error which happened during parsing will occur, namely an undefined var error. */
    // if(dynamic_cast<Identifier*>(binOpExpr->lExpr)->entryRef != nullptr) {
        // cout_str("DesugarAssignModifier...");
    TypeVisitor typeVisitor;
        // dynamic_cast<Identifier*>(binOpExpr->lExpr)->info = info;
    typeVisitor.annotateExpr(binOpExpr->lExpr);
    // }
    // else {
    //     dynamic_cast<Identifier*>(binOpExpr->lExpr)->info = info;
    //
    // }

    assignNode->assignOp = "=";
    assignNode->expr = binOpExpr;
    // cout_str("!!!! DEGING YYYLAL MODIFIER " + std::to_string(info.first_column) + " " + std::to_string(info.first_line));

    assignNode->info = newInfo(info);
    return;
}

/* Desugar do while into a while loop. StmntsList is the list of statements
 * which preceed the do while statement. */
CFWhile *desugarDoWhile(stmntList *smntsList, CFDoWhile *doWhileNode) {
    for(Statement *stmnt : *doWhileNode->stmnts) {
        smntsList->push_back(stmnt);
    }

    CFWhile *whileNode = new CFWhile(doWhileNode->expr, doWhileNode->stmnts, doWhileNode->info);
    whileNode->beforeDesugar = "doWhile";
    return whileNode;
}

/* Desugar for loop into a while loop. StmntsList is the list of statements
 * which preceed the for loop statement. */
CFWhile *desugarFor(stmntList *smntsList, CFFor *forNode) {
    smntsList->push_back(forNode->varDef);
    forNode->stmnts->push_back(forNode->assign);
    CFWhile *whileNode = new CFWhile(forNode->m_expr, forNode->stmnts, forNode->info);
    whileNode->beforeDesugar = "for";
    return whileNode;
}

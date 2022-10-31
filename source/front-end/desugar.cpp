/* Class which desugars statements like k -= 1; to k = k - 1; and
 * translates for loops and do while loops to while loops. */

#include <unordered_map>

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/desugar.h"
#include "../headers/astvisitor.h"
#include "../headers/typeresolution.h"

const std::unordered_map<std::string, int> Desugar::modifiers {
     {"+=", 1}, {"-=", 2}, {"*=", 3}, {"/=", 4}, {"%=", 5}
 };

int Desugar::FindModifier(std::string mod) {
    auto search = modifiers.find(mod);

    if (search != modifiers.end()) {
        return search->second;
    }
    else {
        return 0;
    }
}

void Desugar::DesugarAssignModifier(Assignment *assignNode, Expression *lExpr, std::string op, Expression *rExpr, YYLTYPE *info) {
    BitwiseOpExpr *binOpExpr = new BitwiseOpExpr(lExpr, std::string(1, op.at(0)), rExpr, info);

    /* Annotate the left expression of the binary operation with its type. */
    // TypeVisitor typeVisitor;
    TypeResolution::annotateExpr(binOpExpr->lExpr);
    assignNode->assignOp = "=";
    assignNode->expr = binOpExpr;
    assignNode->info = newInfo(info);
    return;
}

/* Desugar do while into a while loop. StmntsList is the list of statements
 * which preceed the do while statement. */
CFWhile *Desugar::DesugarDoWhile(stmntList *smntsList, CFDoWhile *doWhileNode) {
    for(Statement *stmnt : *doWhileNode->stmnts) {
        smntsList->push_back(stmnt);
    }

    CFWhile *whileNode = new CFWhile(doWhileNode->expr, doWhileNode->stmnts, doWhileNode->info);
    whileNode->beforeDesugar = "doWhile";
    return whileNode;
}

/* Desugar for loop into a while loop. StmntsList is the list of statements
 * which preceed the for loop statement. */
CFWhile *Desugar::DesugarFor(stmntList *smntsList, CFFor *forNode) {
    smntsList->push_back(forNode->varDef);
    forNode->stmnts->push_back(forNode->assign);
    CFWhile *whileNode = new CFWhile(forNode->m_expr, forNode->stmnts, forNode->info);
    whileNode->beforeDesugar = "for";
    return whileNode;
}

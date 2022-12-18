/* Helper class for printing the AST in a pretty way. */

#include "../headers/prettytree.h"

int PrettyTree::currStmntDepth;
bool PrettyTree::visitCFCond;
std::vector<bool> PrettyTree::lastNestedStmnts;
std::vector<bool> PrettyTree::currExprRSubtree;
bool PrettyTree::lastFuncDec;
bool PrettyTree::lastRootStmnt;
bool PrettyTree::lastDepthStmnt;
bool PrettyTree::noStmntsCF;
bool PrettyTree::lastFncCallArg;
bool PrettyTree::inFncCallArgExpr;

/* Init variables of the PrettyTree class. */
void PrettyTree::InitPrettyTree() {
    currStmntDepth = 0;
    visitCFCond = false;
    lastNestedStmnts.clear();
    currExprRSubtree.clear();
    lastFuncDec = false;
    lastRootStmnt = false;
    lastDepthStmnt = false;
    noStmntsCF = false;
    lastFncCallArg = false;
    inFncCallArgExpr = false;
}

/* Return beginning 2 branches for statements. */
std::string PrettyTree::StmntRoots() {
    std::string programBranch = lastFuncDec ? "   " : "│  ";
    std::string rootStmntBranch = "";

    if(lastNestedStmnts.size() == 0) {
        rootStmntBranch = lastRootStmnt ? "└─ " : "├─ ";
    }
    else {
        rootStmntBranch = lastRootStmnt ? "   " : "│  ";
    }
    return programBranch + rootStmntBranch;
}

/* Return beginning 2 branches for expressions. */
std::string PrettyTree::ExprRoots() {
    std::string programBranch = lastFuncDec ? "   " : "│  ";
    std::string rootStmntBranch = "";
    rootStmntBranch = lastRootStmnt ? "   " : "│  ";
    return programBranch + rootStmntBranch;
}

/* Return branches of nested statements. */
std::string PrettyTree::NestedStmntBranches() {
    std::string nestedBranches = "";

    /* For every nested statement, check whether they are the last
     * statement in the list of statements they reside in, and
     * if so print either splitting or no branch, depending on
     * whether it is the last nested statement.
     * If not last nested statement, print splitting branch or
     * continueing branch depending on whehther the current
     * statement is the most nested statement. */
    for(size_t i = 0; i < lastNestedStmnts.size(); i++) {
        if(lastNestedStmnts.at(i)) {
            if(i == lastNestedStmnts.size() - 1) {
                nestedBranches += "└─ ";
                break;
            }
            nestedBranches += "   ";
        }
        else {
            if(i == lastNestedStmnts.size() - 1) {
                nestedBranches += "├─ ";
                break;
            }
            nestedBranches += "│  ";
        }
    }
    return nestedBranches;
}

/* Return branches of nested expressions. */
std::string PrettyTree::NestedExprBranches(Expression *expr) {
    std::string nestedBranches = "";
    for(size_t i = 0; i < lastNestedStmnts.size(); i++) {
        if(lastNestedStmnts.at(i)) {
            if(i == lastNestedStmnts.size() - 1) {
                nestedBranches += "   ";
                break;
            }
            nestedBranches += "   ";
        }
        else {
            nestedBranches += "│  ";
        }
    }

    /* If current expression is not in a expr subtree, split branch
     * or close branch depending on whether we are visiting a conditional
     * epxression of a control flow statement, and if this expression is part of
     * a control flow statement, then check whether that control flow statement
     * has any statements (e.g. While true {} has no statements).  */
    if(currExprRSubtree.size() == 0) {
        if(!noStmntsCF && visitCFCond) {
            return nestedBranches + "├─ ";
        }
        else {
            return nestedBranches + "└─ ";
        }
    }

    /* If we are 1 expression deep, print branches depending on whether we are
     * visiting a control flow condition expr, whether this expr is part of a
     * control flow statement which has statements in its body, and whether
     * is a subtree of a fnc call expr and if so if this expr is the last
     * arg of that fnccall. */
    if(currExprRSubtree.size() == 1) {
        if(visitCFCond && !noStmntsCF) {
            if(inFncCallArgExpr && !lastFncCallArg) {
                std::string finalBranch = currExprRSubtree.at(0) ? "│  ├─ " : "│  ├─ ";
                nestedBranches += finalBranch;
            }
            else {
                std::string finalBranch = currExprRSubtree.at(0) ? "│  ┌─ " : "│  └─ ";
                nestedBranches += finalBranch;
            }
        }
        else {
            if(inFncCallArgExpr && !lastFncCallArg) {
                std::string finalBranch = currExprRSubtree.at(0) ? "│  ├─ " : "   ├─ ";
                nestedBranches += finalBranch;
            }
            else {
                std::string finalBranch = currExprRSubtree.at(0) ? "│  ┌─ " : "   └─ ";
                nestedBranches += finalBranch;
            }
        }

        return nestedBranches;
    }

    for(size_t i = 0; i < currExprRSubtree.size(); i++) {
        if(i == 0) {
            if(visitCFCond) {
                nestedBranches += "│  ";
            }
            else {
                std::string startBranch = currExprRSubtree.at(0) ? "│  " : "   ";
                nestedBranches += startBranch;
            }

        }

        /* Print branches for nested expressions depending on whether the parent
         * expressions and the expression itself branched left or right side. */
        if(i > 0) {
            if(i == 1 && inFncCallArgExpr && !lastFncCallArg) {
               nestedBranches += "│  ";
           }
            else if(currExprRSubtree.at(i - 1) && currExprRSubtree.at(i)) {
                nestedBranches += "   ";
            }
            else if(currExprRSubtree.at(i - 1) && !currExprRSubtree.at(i)) {
                nestedBranches += "│  ";
            }
            else if(!currExprRSubtree.at(i - 1) && currExprRSubtree.at(i)) {
                nestedBranches += "│  ";
            }
            else if(!currExprRSubtree.at(i - 1) && !currExprRSubtree.at(i)) {
                nestedBranches += "   ";
            }

            /* Print final branch for leaf nested expression.  */
            if(i == currExprRSubtree.size() - 1) {
                std::string finalBranch = currExprRSubtree.at(i) ? "┌─ " : "└─ ";
                nestedBranches += finalBranch;
            }
        }
    }

    return nestedBranches;
}

std::string PrettyTree::FncDecBranches() {
    return lastFuncDec ? "└─ " : "├─ ";
}

std::string PrettyTree::AllStmntBranches() {
    return StmntRoots() + NestedStmntBranches();
}

std::string PrettyTree::AllExprBranches(Expression *expr) {
    return ExprRoots() + NestedExprBranches(expr);
}

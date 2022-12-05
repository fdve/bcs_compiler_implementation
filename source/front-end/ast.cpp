#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

#include "../headers/ast.h"
#include "../headers/util.h"
#include "../headers/astvisitor.h"
#include "../headers/semanticsvisitor.h"
#include "../headers/printvisitor.h"
#include "../headers/quadgenvisitor.h"
#include "../headers/desugar.h"
#include "../headers/typeresolution.h"
#include "../headers/symboltable.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"

ASTNode::~ASTNode(){}

void ASTNode::VisitASTNode(ASTVisitor *visitor) {
    if(SemanticsVisitor *semanticsVisitor = dynamic_cast<SemanticsVisitor*>(visitor)) {
        semanticsVisitor->visit(this);
    }
    else if(QuadGenVisitor *tacCodeGenVisitor = dynamic_cast<QuadGenVisitor*>(visitor)) {
        tacCodeGenVisitor->visit(this);
    }
    else if(PrintVisitor *printVisitor = dynamic_cast<PrintVisitor*>(visitor)) {
        printVisitor->visit(this, 0);
    }
    else {
        cout_str("error : unrecognized visitor class in VisitASTNode(...)");
    }
}

Program::Program(FuncDeclList *lhs, FuncDecl *start_fnc, FuncDeclList *rhs) {
    this->lhs = lhs;
    this->start_fnc = start_fnc;
    this->rhs = rhs;
}

stmntList *FunctionBlock::GetStmnts() {
    return this->stmnts;
}

Assignment::Assignment(Identifier *id, std::string assignOp, Expression *expr, YYLTYPE *info) {
    this->id = id;

    /* If the assign modifier is e.g. of type +=, desugar the assignment from x += b to x = x + b.*/
    if(Desugar::FindModifier(assignOp)) {
        Desugar::DesugarAssignModifier(this, id, std::string(1, assignOp.at(0)), expr, info);
        return;
    }

    this->assignOp = assignOp;
    this->expr = expr;
    this->info = newInfo(info);
}

BitwiseOpExpr::BitwiseOpExpr(Expression *lExpr, std::string binOp, Expression *rExpr, YYLTYPE *info) {
    this->lExpr = lExpr;
    this->binOp = binOp;
    this->rExpr = rExpr;
    this->info = newInfo(info);
}

Identifier::Identifier(symTableEntry *symTableEntry)
: type(symTableEntry->type), name(symTableEntry->name), entryRef(symTableEntry){}

/* Create a character constant node. Check if the nodes character is legitimate. */
CharConst::CharConst(std::string value, YYLTYPE *info) {
    /* If invalid char const has been defined, report error message.  */
    if(!CheckCharLegitimacy(value)) {
        ErrLog::InsertMsg(ErrLog::IllegitimateChar(info, value));
        this->value = -1;
        return;
    }

    this->value = ConvertCharStrToInt(value);
}

bool CharConst::CheckCharLegitimacy(std::string charString) {
    /* Legimate char constants need to start and end with a quotation mark.*/
    if((int)charString.at(0) != 39 || (int)charString.at(charString.size() -1) != 39) {
        return false;
    }

    std::string specialAsciiChars = "0trn";

    /* Legimate char constants are either of size 3 or 4 (e.g. 'a', '/n'). */
    /* '\n', \t, \r, are all considered 1 chars. \0 is considered 0 chars */
    if(charString.size() == 3) {
        const char &c = charString.at(1);

        /* Check normal printable ascii chars */
        if((int)c < 32 || (int)c > 126) {
            return false;
        }

        return true;
    }
    else if(charString.size() == 4) {
        if((int)charString.at(1) != 92) {
            return false;
        }
        for(const char &c : specialAsciiChars) {
            if(charString.at(2) == c) {
                return true;
            }
        }
        return false;
    }
    else {
        return false;
    }
}

int CharConst::ConvertCharStrToInt(std::string charString) {
    if(charString.size() != 3 && charString.size() != 4) {
        cout_str("error : CharConst::ConvertCharStrToInt(str), charString size should be either 3 or 4. ");
    }
    else {
        /* Check the 3rd character in the special ASCII literals,
         * then return correct ASCII value of the literal itself.
         * (e.g. n in '\n' is 110, so the value 13 (ASCII newline value) is returned. */
        if(charString.size() == 4) {
            switch ((int)charString.at(2)) {
                case 48:
                    return 0;
                case 116:
                    return 9;
                case 114:
                    return 13;
                case 110:
                    return 10;
                default :
                    cout_str("error : CharConst::ConvertCharStrToInt, unrecognized ASCII value, charString size 4");
            }
        }
        else {
            return (int)charString.at(1);
        }
    }
    return 1;
}

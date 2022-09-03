#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

#include "../headers/ast.h"
#include "../headers/util.h"
#include "../headers/astvisitors.h"
#include "../headers/desugar.h"
#include "../headers/typeresolution.h"
#include "../headers/symboltable.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"

ASTNode::~ASTNode(){}

void ASTNode::VisitASTNode(ASTVisitor *visitor) {
    if(TypeVisitor *typeVisitor = dynamic_cast<TypeVisitor*>(visitor)) {
        typeVisitor->visit(this);
    }
    else if(TACGenVisitor *tacCodeGenVisitor = dynamic_cast<TACGenVisitor*>(visitor)) {
        tacCodeGenVisitor->visit(this);
    }
    else if(PrintVisitor *printVisitor = dynamic_cast<PrintVisitor*>(visitor)) {
        printVisitor->visit(this, 0);
    }
    else {
        cout_str("error : unrecognized visitor class in VisitASTNode(...)");
    }
}

/* Contains all the functions of our program, and represents one program.
 * Constructing programs according to the parse rules, with optional functions
 * on the left side in the tree, mandatory start function somewhere in the middle,
 * and optional functions on the right hand side of the tree.
    */
Program::Program(funcDeclList *lhs, FuncDecl *start_fnc, funcDeclList *rhs) {
    this->lhs = lhs;
    this->start_fnc = start_fnc;
    this->rhs = rhs;
}

/* Function declaration constructor for a start() func dec method. */
// FuncDecl::FuncDecl(stmntList *fncBlock) :
//     FuncDecl(std::string("int"), new Identifier("start"), nullptr, fncBlock) {}

// FuncDeclParam::FuncDeclParam(std::string type, Identifier *id) {
//     this->type = type;
//     this->id = id;
// }

// VariableDef::VariableDef(std::string type, Identifier *id, Expression *expr) {
//     this->type = type;
//     this->id = id;
//     this->expr = expr;
// }

/* - [] what does the if do?   */
Assignment::Assignment(Identifier *id, std::string assignOp, Expression *expr, YYLTYPE *info) {
    this->id = id;

    /* -[] bin op type of minus = float? */

    // cout_str("assignOp in assignment constructor : " + assignOp);

    /* - [] move tihs to desugar.cpp? */
    if(findModifier(assignOp)) {
        DesugarAssignModifier(this, id, std::string(1, assignOp.at(0)), expr, info);

        // BitwiseOpExpr *binOpExpr = new BitwiseOpExpr(this->id, std::string(1, assignOp.at(0))
        // , this->expr = expr, info);
        // this->assignOp = "=";
        // this->expr = binOpExpr;




        // cout_str("!!!! DEGING YYYLAL MODIFIER " + std::to_string(info.first_column) + " " + std::to_string(info.first_line));

        // this->info = newInfo(info);
        return;
    }

    this->assignOp = assignOp;
    this->expr = expr;
    // cout_str("!!!! DEGING YYYLAL " + std::to_string(info.first_column) + " " + std::to_string(info.first_line));

    this->info = newInfo(info);
}

// CFIf::CFIf(Expression *expr, stmntList *ifStmnts, stmntList *elseStmnts, YYLTYPE *info) {
//     this->expr = expr;
//     this->ifStmnts = ifStmnts;
//     this->elseStmnts = elseStmnts;
//     this->info = info;
//
//     cout_str("in CF::CF construction ast.cpp, printing info information.");
//     printInfo(info);
// }

// UnaryOpExpr::UnaryOpExpr(char , Expression *expr) {
//     this-> = ;
//     this->expr = expr;
// }

BitwiseOpExpr::BitwiseOpExpr(Expression *lExpr, std::string bin_op, Expression *rExpr, YYLTYPE *info) {
    // cout_str("CREATING BIN OP EXPRESSION WITH BIN OP " + bin_op);
    //
    // if(Identifier *identifier = dynamic_cast<Identifier*>(lExpr)) {
    //     if(identifier->entryRef == nullptr) {
    //         std::cout << spaced_str(5) << "Has no sym table entry" << std::endl;
    //     }
    //     else {
    //         std::cout << spaced_str(5) << "Has sym table entry, printing entry : ";
    //         SymbolTable::PrintEntry(*(identifier->entryRef), 1);
    //     }
    // }
    //
    // if(Identifier *identifier = dynamic_cast<Identifier*>(rExpr)) {
    //     if(identifier->entryRef == nullptr) {
    //         std::cout << spaced_str(5) << "Has no sym table entry" << std::endl;
    //     }
    //     else {
    //         std::cout << spaced_str(5) << "Has sym table entry, printing entry : ";
    //         SymbolTable::PrintEntry(*(identifier->entryRef), 1);
    //     }
    // }


    this->lExpr = lExpr;
    this->bin_op = bin_op;
    this->rExpr = rExpr;
    this->info = newInfo(info);
}

Identifier::Identifier(symTableEntry *symTableEntry)
: type(symTableEntry->type), name(symTableEntry->name), entryRef(symTableEntry){}

/* -[] move somewhere else? */
bool CharConst::CheckCharLegitimacy(std::string charString) {
    /* Legimate char constants need to start and end with a quotation mark.*/
    if((int)charString.at(0) != 39 || (int)charString.at(charString.size() -1) != 39) {
        return false;
    }

    std::string specialAsciiChars = "0trn";

    /* Legimate char constants are either of size 3 or 4 (e.g. 'a', '/n'). */
        // !!! -> '\n', \t, \r, are all considered 1 char. \0 is considered 0 char
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

/* -[] move somewhere else? */

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
                    cout_str("error : CharConst::ConvertCharStrToInt, unrecognized ASCII value of charString size 4");
            }
        }
        else {
            return (int)charString.at(1);
        }
    }
    return -2;
}
/* -[] move somewhere else? */

/* Create a character constant node. Check if the nodes character is legitimate. */
CharConst::CharConst(std::string value) {
    /* If invalid char const has been defined, report error message to stream of compiler error messages.  */
    if(!CheckCharLegitimacy(value)) {
        ErrorLogger::InsertMsg(ErrorLogger::IllegitimateChar(value));
        this->value = -1;
        return;
    }

    this->value = ConvertCharStrToInt(value);
}

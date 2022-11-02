/* Visit the AST and generate three-address code instructions, implemented as
 * quadruples. The instructions are stored in Quad::QuadInstrucs. */

#include <fstream>
#include <stack>

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/astvisitor.h"
#include "../headers/quadgenvisitor.h"
#include "../headers/symboltable.h"
#include "../headers/quad.h"
#include "../headers/asmcodegen.h"

void QuadGenVisitor::visit(ASTNode *node) {
    check_nullptr_throws(dynamic_cast<Program*>(node), "Program_node");
    visit(dynamic_cast<Program*>(node));
}

/* Visit the tree, and generate Quad while traversing it. */
void QuadGenVisitor::visit(Program *node) {
    /* Reset quad temporary and label counters. */
    Quad::ResetQuadCounters();

    Program *progNode = dynamic_cast<Program*>(node);

    for(FuncDecl *funcDec : *progNode->lhs) {
        visit(funcDec);
    }

    visit(progNode->start_fnc);

    for(FuncDecl *funcDec : *progNode->rhs) {
        visit(funcDec);
    }
}

void QuadGenVisitor::visit(FuncDecl *funcDec) {
    if(funcDec->id == nullptr) {
        return;
    }

    std::string funcLabel = (funcDec->id->name == "start") ? "main" : funcDec->id->name;

    quadArg quad_arg_label = {true, nullptr, "", funcLabel, quadArgType::label};
    QuadInstruc instr = QuadInstruc(quadOpType::funclabel, quadArg{}, quadArg{}, quad_arg_label);
    Quad::QuadInstrucs.push_back(instr);

    if(funcDec->paramList != nullptr) {
        size_t paramListSize = funcDec->paramList->size();
        size_t numParams = 0;

        for(FuncDeclParam *funcDeclParam : *funcDec->paramList) {
            numParams++;
            if(numParams == paramListSize) {
                visit(funcDeclParam, true);
            }
            else {
                visit(funcDeclParam, false);
            }
        }
    }

    for(Statement *statement : *funcDec->fncBlock) {
        visit(statement);
    }

    /* If there is no return call present in a void return function,
     * add a Quad return instruction manually. */
    if(funcDec->type == "void") {
        bool returnCallPresent = false;
        for(Statement *statement : *funcDec->fncBlock) {
             if(dynamic_cast<ReturnCall*>(statement) != nullptr) {
                 returnCallPresent = true;
                 break;
             }
        }

        if(!returnCallPresent) {
            quadArg arg1;
            arg1.label = "void";
            QuadInstruc instr = QuadInstruc(quadOpType::ireturn, arg1, quadArg{}, quadArg{});
            Quad::QuadInstrucs.push_back(instr);
        }
    }
}

void QuadGenVisitor::visit(FuncDeclParam *funcDeclParam, bool lastParam) {
    quadArg arg1;
    arg1.entryRef = funcDeclParam->id->entryRef;
    arg1.name = arg1.entryRef->name;
    arg1.label = "decl_param";

    quadArg arg2;

    /* If current param is the last param, store this in the arg2 label field. */
    if(lastParam) {
        arg2.label = "lastParam";
    }

    QuadInstruc instr = QuadInstruc(quadOpType::param, arg1, arg2, quadArg{});
    Quad::QuadInstrucs.push_back(instr);
}

void QuadGenVisitor::visit(Statement *stmnt) {
    if(FunctionBlock *funcBlock = dynamic_cast<FunctionBlock*>(stmnt)) {
        for(Statement *statement : *funcBlock->stmnts) {
            visit(statement);
        }
    }
    else if(VariableDef *varDef = dynamic_cast<VariableDef*>(stmnt)) {
        symTableEntry *exprTempoRef = visit(varDef->expr);
        symTableEntry *varEntryRef = varDef->id->entryRef;

        /* Create a constant quadruple argument. */
        quadArg arg1 = {false, nullptr, exprTempoRef->name, "", quadArgType::variable, exprTempoRef};
        quadArg result = {false, nullptr, varDef->id->name, "", quadArgType::variable, varEntryRef};

        quadOpType qType = (exprTempoRef->type == "float") ? quadOpType::fstore : quadOpType::istore;

        /* Create the quadruple instruction with a constant quadruple argument,
         * and save the entry symbol table key with it. */
        QuadInstruc instr = QuadInstruc(qType, arg1, quadArg{}, result);
        Quad::QuadInstrucs.push_back(instr);
    }
    else if(Assignment *assign = dynamic_cast<Assignment*>(stmnt)) {
        symTableEntry *exprTempoRef = visit(assign->expr);
        symTableEntry *varEntryRef = assign->id->entryRef;

        quadArg arg1 = {false, nullptr, exprTempoRef->name, "", quadArgType::variable, exprTempoRef};
        quadArg result = {false, nullptr, assign->id->name, "", quadArgType::variable, varEntryRef};

        quadOpType qType = (exprTempoRef->type == "float") ? quadOpType::fstore : quadOpType::istore;

        QuadInstruc instr = QuadInstruc(qType, arg1, quadArg{}, result);
        Quad::QuadInstrucs.push_back(instr);
    }
    else if(ExprStmnt *exprStmnt = dynamic_cast<ExprStmnt*>(stmnt)) {
        if(exprStmnt->expr == nullptr) {
            cout_str("QuadGenVisitor::visiting exprStmnt, expr is a nullptr.");
        }
        visit(exprStmnt->expr);
    }
    else if(ReturnCall *returnCall = dynamic_cast<ReturnCall*>(stmnt)) {
        if(returnCall->expr != nullptr) {
            symTableEntry *entryRef = visit(returnCall->expr);
            quadArg arg1;
            arg1.name = entryRef->name;
            arg1.entryRef = entryRef;

            quadOpType qOpType = (returnCall->expr->getType() == "float") ? quadOpType::freturn : quadOpType::ireturn;
            QuadInstruc instr = QuadInstruc(qOpType, arg1, quadArg{}, quadArg{});
            Quad::QuadInstrucs.push_back(instr);
        }
        else {
            quadArg arg1;
            arg1.label = "void";
            QuadInstruc instr = QuadInstruc(quadOpType::ireturn, quadArg{}, quadArg{}, quadArg{});
            Quad::QuadInstrucs.push_back(instr);
        }
    }
    else if(CFIf *cfIf = dynamic_cast<CFIf*>(stmnt)) {
        symTableEntry *exprTemporaryRef = visit(cfIf->expr);

        /* Get new label to jump to for control flow Quad and assign to result arg. */
        std::string newCFLabel = Quad::GetNewCFLabel();
        quadArg resultArg;
        resultArg.label = newCFLabel;

        quadArg conditional = quadArg{0, nullptr, exprTemporaryRef->name, "",
                               quadArgType::variable, exprTemporaryRef};

        QuadInstruc instr = QuadInstruc(quadOpType::jmp, conditional, quadArg{}, resultArg);
        Quad::QuadInstrucs.push_back(instr);

        if(!check_nullptr(cfIf->ifStmnts, "cfIf->ifStmns")) {
            for(Statement *stmnt : *cfIf->ifStmnts) {
                visit(stmnt);
            }
        }

        /* If the value of the conditional temporary is false, jump to either
         * (1) else statements if such statements are present;
         * (2) if no else code, jump to first code outside the if statement. */
        if(!check_nullptr(cfIf->elseStmnts, "cfIf->elseStmnts")) {
            QuadInstruc instr = QuadInstruc(quadOpType::jmplabel, quadArg{}, quadArg{}, resultArg);
            Quad::QuadInstrucs.push_back(instr);

            for(Statement *stmnt : *cfIf->elseStmnts) {
                visit(stmnt);
            }
        }
        else {
            QuadInstruc instr = QuadInstruc(quadOpType::jmplabel, quadArg{}, quadArg{}, resultArg);
            Quad::QuadInstrucs.push_back(instr);
        }
    }
    else if(CFWhile *cfWhile = dynamic_cast<CFWhile*>(stmnt)) {
        /* Make label to jump to when loop condition is still true. */
        std::string whileLoopTopLabel = Quad::GetNewCFLabel();
        quadArg qTopLabelArg;
        qTopLabelArg.label = whileLoopTopLabel;
        QuadInstruc instr = QuadInstruc(quadOpType::jmplabel, quadArg{}, quadArg{}, qTopLabelArg);
        Quad::QuadInstrucs.push_back(instr);


        symTableEntry *exprTemporaryRef = visit(cfWhile->expr);

        /* Get new label to jump to for control flow Quad and assign to result arg. */
        std::string whileLoopEndLabel = Quad::GetNewCFLabel();
        quadArg qEndLabelArg;
        qEndLabelArg.label = whileLoopEndLabel;
        quadArg conditional = quadArg{0, nullptr, exprTemporaryRef->name, "",
                               quadArgType::variable, exprTemporaryRef};

        QuadInstruc jmpInstrucEnd = QuadInstruc(quadOpType::jmp, conditional, quadArg{}, qEndLabelArg);
        Quad::QuadInstrucs.push_back(jmpInstrucEnd);

        for(Statement *stmnt : *cfWhile->stmnts) {
            visit(stmnt);
        }

        /* Jump back to the top of the while loop. */
        QuadInstruc jmpInstrucTop = QuadInstruc(quadOpType::jmp, quadArg{}, quadArg{}, qTopLabelArg);
        Quad::QuadInstrucs.push_back(jmpInstrucTop);

        /* Make label to jump to if the loop condition is not true. */
        QuadInstruc labelInstrucEnd = QuadInstruc(quadOpType::jmplabel, quadArg{}, quadArg{}, qEndLabelArg);
        Quad::QuadInstrucs.push_back(labelInstrucEnd);
    }
    else {
        cout_str("error : does not detect a derived class of type stmnt class in QuadGenVisitor");
    }
}

symTableEntry* QuadGenVisitor::visit(Expression *expr) {
    if(UnaryOpExpr *unaryOpExpr = dynamic_cast<UnaryOpExpr*>(expr)) {
        symTableEntry *exprTemporaryRef = visit(unaryOpExpr->expr);

        std::string type = exprTemporaryRef->type;
        std::string resultTemporary = Quad::GetNewTemporary();
        symTableEntry resultTemporaryEntry = {type, resultTemporary, nullptr};

        SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
        symTableEntry *resultTemporaryRef = &SymbolTable::temporariesSymTab[resultTemporary];

        quadOpType qUnaryOpType;
        if(unaryOpExpr->unaryOp == '!') {
            qUnaryOpType = quadOpType::iunnot;
        }
        else {
            if(type == "int") {
                qUnaryOpType = quadOpType::iunmin;
            }
            else if(type == "float") {
                qUnaryOpType = quadOpType::funmin;
            }
            else {
                cout_str("error : unrecognized type in unaryOpExpr, tacgenvisitor expr. ");
            }
        }

        QuadInstruc instr = QuadInstruc(qUnaryOpType, exprTemporaryRef, nullptr, resultTemporaryRef);
        Quad::QuadInstrucs.push_back(instr);

        return resultTemporaryRef;
    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        symTableEntry *entryRefLHS = visit(binOpExpr->lExpr);
        symTableEntry *entryRefRHS = visit(binOpExpr->rExpr);

        std::string type = binOpExpr->type;
        std::string resultTemporary = Quad::GetNewTemporary();

        symTableEntry resultTemporaryEntry = {type, resultTemporary, nullptr};
        SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
        symTableEntry *resultEntryRef = &SymbolTable::temporariesSymTab[resultTemporary];

        QuadInstruc instr = QuadInstruc(Quad::SourceOptoQuadOp(binOpExpr->binOp, entryRefLHS->type),
        entryRefLHS, entryRefRHS, resultEntryRef);
        Quad::QuadInstrucs.push_back(instr);

        return resultEntryRef;
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        int funcCallParamSize = (funcCall->params == nullptr) ? 0 : funcCall->params->size();

        /* For every param in a funcCall, add a param quad instruc. */
        for (int i = 0; i < funcCallParamSize; i++) {
            quadArg arg1;
            arg1.entryRef = visit(funcCall->params->at(i));
            arg1.name = arg1.entryRef->name;

            QuadInstruc instr = QuadInstruc(quadOpType::param, arg1, quadArg{}, quadArg{});
            Quad::QuadInstrucs.push_back(instr);
        }

        quadArg arg1;
        arg1.type = quadArgType::label;
        arg1.label = funcCall->id->name == "start" ? "main" : funcCall->id->name;

        quadArg arg2;

        /* Store the amount of params in the label field of the second quad arg.  */
        arg2.label = std::to_string(funcCallParamSize);

        quadArg result;

        /* If func call returns non void type, make a symtable entry for it and return
         * a reference to it. */
        if(funcCall->type != "void") {
            std::string resultTemporary = Quad::GetNewTemporary();
            symTableEntry resultTemporaryEntry = {funcCall->type, resultTemporary, nullptr};
            SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
            symTableEntry *resultEntryRef = &SymbolTable::temporariesSymTab[resultTemporary];

            result.entryRef = resultEntryRef;
            result.name = resultEntryRef->name;

            QuadInstruc instr = QuadInstruc(quadOpType::call, arg1, arg2, result);
            Quad::QuadInstrucs.push_back(instr);
            return resultEntryRef;
        }

        QuadInstruc instr = QuadInstruc(quadOpType::call, arg1, arg2, result);
        Quad::QuadInstrucs.push_back(instr);

        /* If the function call is of type void, then return nothing, since
         * there will be no return value associated with the call. */
        return nullptr;
    }
    else if(Identifier *identifier = dynamic_cast<Identifier*>(expr)) {
        return identifier->entryRef;
    }
    else if(dynamic_cast<IntConst*>(expr) != nullptr) {
        return ConstantQuadGen(expr);
    }
    else if(dynamic_cast<FloatConst*>(expr) != nullptr) {
        return ConstantQuadGen(expr);
    }
    else if(dynamic_cast<BoolConst*>(expr) != nullptr) {
        return ConstantQuadGen(expr);
    }
    else if(dynamic_cast<CharConst*>(expr) != nullptr) {
        return ConstantQuadGen(expr);
    }
    else {
        cout_str("error : unrecognized expression in QuadGenVisitor::visit, QuadGenVisitor");
        return nullptr;
    }
}

/* Generate quad instructions for constants, such as int, bool. */
symTableEntry* QuadGenVisitor::ConstantQuadGen(Expression *expr) {
    std::string type;
    quadArgType qArgType;
    quadOpType qOpType = quadOpType::istore;

    if(dynamic_cast<IntConst*>(expr) != nullptr) {
        qArgType = quadArgType::intConst;
        type = "int";
    }
    else if(dynamic_cast<FloatConst*>(expr) != nullptr) {
        qArgType = quadArgType::floatConst;
        qOpType = quadOpType::fstore;
        type = "float";
    }
    else if(dynamic_cast<BoolConst*>(expr) != nullptr) {
        qArgType = quadArgType::boolConst;
        type = "bool";
    }
    else if(dynamic_cast<CharConst*>(expr) != nullptr) {
        qArgType = quadArgType::charConst;
        type = "char";
    }
    else {
        cout_str("error : in tacGenVisitor, unrecognized expression in ConstantQuadGen...");
        return nullptr;
    }

    /* Get a new temporary variable. */
    std::string resultTemporary = Quad::GetNewTemporary();

    /* Create a new symbol table entry for the temporary. */
    symTableEntry resultTemporaryEntry = {type, resultTemporary, expr};

    SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
    symTableEntry *resultEntryRef = &SymbolTable::temporariesSymTab[resultTemporary];

    /* Create a constant quadruple argument. */
    quadArg constValArg = {true, expr, "", "", qArgType};
    quadArg tempoResultArg = {false, nullptr, resultTemporary, "", quadArgType::variable, resultEntryRef};

    /* Create the quadruple instruction with a constant quadruple argument,
     * and save the entry symbol table key with it. */
    QuadInstruc instr = QuadInstruc(qOpType, constValArg, quadArg{}, tempoResultArg);
    Quad::QuadInstrucs.push_back(instr);
    return resultEntryRef;
}

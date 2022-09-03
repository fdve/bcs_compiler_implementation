/* Visit the AST and generate three-address code (abbreviation TAC). */

#include <fstream>
#include <stack>

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/astvisitors.h"
#include "../headers/symboltable.h"
#include "../headers/tac.h"
#include "../headers/asmcodegen.h"

void TACGenVisitor::visit(ASTNode *node) {
    check_nullptr_throws(dynamic_cast<Program*>(node), "Program_node");
    visit(dynamic_cast<Program*>(node));
}

/* Visit the tree, and generate TAC while traversing it. */
void TACGenVisitor::visit(Program *node) {
    /* Reset tac temporary and label counters. */
    TAC::ResetTACCounters();

    Program *progNode = dynamic_cast<Program*>(node);

    for(FuncDecl *funcDec : *progNode->lhs) {
        visit(funcDec);
    }

    visit(progNode->start_fnc);

    for(FuncDecl *funcDec : *progNode->rhs) {
        visit(funcDec);
    }
}

void TACGenVisitor::visit(FuncDecl *funcDec) {
    if(funcDec->id == nullptr) {
        return;
    }

    std::string funcLabel = (funcDec->id->name == "start") ? "main" : funcDec->id->name;
    // ASMCodeGenerator::asmGeneratedCode << funcDec->id->name << ":\n";

    quadArg quad_arg_label = {true, nullptr, "", funcLabel, quadArgType::label};
    TACInstruction instr = TACInstruction(quadOpType::funclabel, quadArg{}, quadArg{}, quad_arg_label);
    TAC::tacInstructions.push_back(instr);

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
     * add a TAC return instruction manually. */
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
            TACInstruction instr = TACInstruction(quadOpType::ireturn, arg1, quadArg{}, quadArg{});
            TAC::tacInstructions.push_back(instr);
        }
    }
}

void TACGenVisitor::visit(FuncDeclParam *funcDeclParam, bool lastParam) {
    quadArg arg1;
    arg1.entryRef = funcDeclParam->id->entryRef;
    arg1.name = arg1.entryRef->name;
    arg1.label = "decl_param";

    quadArg arg2;
    // arg2.label = std::to_string(paramNum);

    /* If current param is the last param, store this in the arg2 label field. */
    if(lastParam) {
        arg2.label = "lastParam";
    }

    TACInstruction instr = TACInstruction(quadOpType::param, arg1, arg2, quadArg{});
    TAC::tacInstructions.push_back(instr);
}

void TACGenVisitor::visit(Statement *stmnt) {
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
        TACInstruction instr = TACInstruction(qType, arg1, quadArg{}, result);
        TAC::tacInstructions.push_back(instr);
    }
    else if(Assignment *assign = dynamic_cast<Assignment*>(stmnt)) {
        symTableEntry *exprTempoRef = visit(assign->expr);
        symTableEntry *varEntryRef = assign->id->entryRef;

        quadArg arg1 = {false, nullptr, exprTempoRef->name, "", quadArgType::variable, exprTempoRef};
        quadArg result = {false, nullptr, assign->id->name, "", quadArgType::variable, varEntryRef};

        quadOpType qType = (exprTempoRef->type == "float") ? quadOpType::fstore : quadOpType::istore;

        TACInstruction instr = TACInstruction(qType, arg1, quadArg{}, result);
        TAC::tacInstructions.push_back(instr);
    }
    else if(ExprStmnt *exprStmnt = dynamic_cast<ExprStmnt*>(stmnt)) {
        if(exprStmnt->expr == nullptr) {
            cout_str("TACGenVisitor::visiting exprStmnt, expr is a nullptr.");
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
            TACInstruction instr = TACInstruction(qOpType, arg1, quadArg{}, quadArg{});
            TAC::tacInstructions.push_back(instr);
        }
        else {
            quadArg arg1;
            arg1.label = "void";
            TACInstruction instr = TACInstruction(quadOpType::ireturn, quadArg{}, quadArg{}, quadArg{});
            TAC::tacInstructions.push_back(instr);
        }
    }
    else if(CFIf *cfIf = dynamic_cast<CFIf*>(stmnt)) {
        symTableEntry *exprTemporaryRef = visit(cfIf->expr);

        /* Get new label to jump to for control flow TAC and assign to result arg. */
        std::string newCFLabel = TAC::GetNewCFLabel();
        quadArg resultArg;
        resultArg.label = newCFLabel;

        quadArg conditional = quadArg{0, nullptr, exprTemporaryRef->name, "",
                               quadArgType::variable, exprTemporaryRef};

        TACInstruction instr = TACInstruction(quadOpType::jmp, conditional, quadArg{}, resultArg);
        TAC::tacInstructions.push_back(instr);

        if(!check_nullptr(cfIf->ifStmnts, "cfIf->ifStmns")) {
            for(Statement *stmnt : *cfIf->ifStmnts) {
                visit(stmnt);
            }
        }

        /* If the value of the conditional temporary is false, jump to either
         * (1) else statements if such statements are present;
         * (2) if no else code, jump to first code outside the if statement. */
        if(!check_nullptr(cfIf->elseStmnts, "cfIf->elseStmnts")){
            TACInstruction instr = TACInstruction(quadOpType::jmplabel, quadArg{}, quadArg{}, resultArg);
            TAC::tacInstructions.push_back(instr);

            for(Statement *stmnt : *cfIf->elseStmnts) {
                visit(stmnt);
            }
        }
        else {
            TACInstruction instr = TACInstruction(quadOpType::jmplabel, quadArg{}, quadArg{}, resultArg);
            TAC::tacInstructions.push_back(instr);
        }
    }
    else if(CFWhile *cfWhile = dynamic_cast<CFWhile*>(stmnt)) {
        /* Make label to jump to when loop condition is still true. */
        std::string whileLoopTopLabel = TAC::GetNewCFLabel();
        quadArg qTopLabelArg;
        qTopLabelArg.label = whileLoopTopLabel;
        TACInstruction instr = TACInstruction(quadOpType::jmplabel, quadArg{}, quadArg{}, qTopLabelArg);
        TAC::tacInstructions.push_back(instr);


        symTableEntry *exprTemporaryRef = visit(cfWhile->expr);

        /* Get new label to jump to for control flow TAC and assign to result arg. */
        std::string whileLoopEndLabel = TAC::GetNewCFLabel();
        quadArg qEndLabelArg;
        qEndLabelArg.label = whileLoopEndLabel;
        quadArg conditional = quadArg{0, nullptr, exprTemporaryRef->name, "",
                               quadArgType::variable, exprTemporaryRef};

        TACInstruction jmpInstrucEnd = TACInstruction(quadOpType::jmp, conditional, quadArg{}, qEndLabelArg);
        TAC::tacInstructions.push_back(jmpInstrucEnd);

        for(Statement *stmnt : *cfWhile->stmnts) {
            visit(stmnt);
        }

        /* Jump back to the top of the while loop. */
        TACInstruction jmpInstrucTop = TACInstruction(quadOpType::jmp, quadArg{}, quadArg{}, qTopLabelArg);
        TAC::tacInstructions.push_back(jmpInstrucTop);

        /* Make label to jump to if the loop condition is not true. */
        TACInstruction labelInstrucEnd = TACInstruction(quadOpType::jmplabel, quadArg{}, quadArg{}, qEndLabelArg);
        TAC::tacInstructions.push_back(labelInstrucEnd);
    }
    else {
        cout_str("error : does not detect a derived class of type stmnt class in TACGenVisitor");
    }
}

symTableEntry* TACGenVisitor::visit(Expression *expr) {
    if(UnaryOpExpr *unaryOpExpr = dynamic_cast<UnaryOpExpr*>(expr)) {
        symTableEntry *exprTemporaryRef = visit(unaryOpExpr->expr);

        std::string type = exprTemporaryRef->type;
        std::string resultTemporary = TAC::GetNewTemporary();
        symTableEntry resultTemporaryEntry = {type, resultTemporary, 0, nullptr};

        SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
        symTableEntry *resultTemporaryRef = &SymbolTable::temporariesSymTab[resultTemporary];

        // quadOpType qUnaryOpType = (unaryOpExpr->unaryOp == '!') ? quadOpType::iunnot : quadOpType::iunmin;

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

        TACInstruction instr = TACInstruction(qUnaryOpType, exprTemporaryRef, nullptr, resultTemporaryRef);
        TAC::tacInstructions.push_back(instr);

        return resultTemporaryRef;
    }
    else if(BitwiseOpExpr *binOpExpr = dynamic_cast<BitwiseOpExpr*>(expr)) {
        symTableEntry *entryRefLHS = visit(binOpExpr->lExpr);
        symTableEntry *entryRefRHS = visit(binOpExpr->rExpr);

        std::string type = binOpExpr->type;
        std::string resultTemporary = TAC::GetNewTemporary();

        symTableEntry resultTemporaryEntry = {type, resultTemporary, 0, nullptr};
        SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
        symTableEntry *resultEntryRef = &SymbolTable::temporariesSymTab[resultTemporary];

        TACInstruction instr = TACInstruction(TAC::SourceOptoQuadOp(binOpExpr->bin_op, entryRefLHS->type),
        entryRefLHS, entryRefRHS, resultEntryRef);
        TAC::tacInstructions.push_back(instr);

        return resultEntryRef;
    }
    else if(FuncCall *funcCall = dynamic_cast<FuncCall*>(expr)) {
        // std::cout << "Funccall id:" << funcCall->id->name << std::endl;

        int funcCallParamSize = (funcCall->params == nullptr) ? 0 : funcCall->params->size();

        // for (int i = funcCallParamSize - 1; i >= 0; i--) {
        for (int i = 0; i < funcCallParamSize; i++) {
            quadArg arg1;
            arg1.entryRef = visit(funcCall->params->at(i));
            arg1.name = arg1.entryRef->name;

            TACInstruction instr = TACInstruction(quadOpType::param, arg1, quadArg{}, quadArg{});
            TAC::tacInstructions.push_back(instr);
        }

        quadArg arg1;
        arg1.type = quadArgType::label;
        arg1.label = funcCall->id->name == "start" ? "main" : funcCall->id->name;

        quadArg arg2;

        /* Store the amount of params in the label field of the second quad arg.  */
        arg2.label = std::to_string(funcCallParamSize);

        // cout_str("HELLOW, FUNCALL TYPE = " + funcCall->type);

        quadArg result;
        if(funcCall->type != "void") {
            std::string resultTemporary = TAC::GetNewTemporary();
            symTableEntry resultTemporaryEntry = {funcCall->type, resultTemporary, 0, nullptr};
            SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
            symTableEntry *resultEntryRef = &SymbolTable::temporariesSymTab[resultTemporary];

            result.entryRef = resultEntryRef;
            result.name = resultEntryRef->name;

            TACInstruction instr = TACInstruction(quadOpType::call, arg1, arg2, result);
            TAC::tacInstructions.push_back(instr);
            return resultEntryRef;
        }

        TACInstruction instr = TACInstruction(quadOpType::call, arg1, arg2, result);
        TAC::tacInstructions.push_back(instr);

        /* If the function call is of type void, then return nothing, since
         * there will be no return value associated with the call. */
        return nullptr;
    }
    else if(Identifier *identifier = dynamic_cast<Identifier*>(expr)) {
        return identifier->entryRef;
    }
    else if(dynamic_cast<IntConst*>(expr) != nullptr) {
        return ConstantTACGen(expr);
    }
    else if(dynamic_cast<FloatConst*>(expr) != nullptr) {
        return ConstantTACGen(expr);
    }
    else if(dynamic_cast<BoolConst*>(expr) != nullptr) {
        return ConstantTACGen(expr);
    }
    else if(dynamic_cast<CharConst*>(expr) != nullptr) {
        return ConstantTACGen(expr);
    }
    else {
        cout_str("error : unrecognized expression in GenerateTACExpr(...), TACGenVisitor");
    }
}

symTableEntry* ConstantTACGen(Expression *expr) {
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
        cout_str("error : in tacGenVisitor, unrecognized expression in ConstantTACGen...");
        return nullptr;
    }

    /* Get a new temporary variable. */
    std::string resultTemporary = TAC::GetNewTemporary();

    /* Create a new symbol table entry for the temporary. */
    symTableEntry resultTemporaryEntry = {type, resultTemporary, 0, expr};
    SymbolTable::temporariesSymTab[resultTemporary] = resultTemporaryEntry;
    symTableEntry *resultEntryRef = &SymbolTable::temporariesSymTab[resultTemporary];

    /* Create a constant quadruple argument. */
    quadArg constValArg = {true, expr, "", "", qArgType};
    quadArg tempoResultArg = {false, nullptr, resultTemporary, "", quadArgType::variable, resultEntryRef};

    /* Create the quadruple instruction with a constant quadruple argument,
     * and save the entry symbol table key with it. */
    TACInstruction instr = TACInstruction(qOpType, constValArg, quadArg{}, tempoResultArg);
    TAC::tacInstructions.push_back(instr);
    return resultEntryRef;
}

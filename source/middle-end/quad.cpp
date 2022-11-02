/* Implementation file of the three-address code. The Quad instructions
 * are stored in the static class field "QuadInstrucs."  */

#include <sstream>

#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/symboltable.h"
#include "../headers/quad.h"

int Quad::temporaryCounter = 1;
int Quad::CFLabelCounter = 1;
std::vector<QuadInstruc> Quad::QuadInstrucs;

QuadInstruc::QuadInstruc(quadOpType op, symTableEntry *arg1Ref, symTableEntry *arg2Ref, symTableEntry *resultRef) {
    this->op = op;
    this->arg1 = quadArg{0, nullptr, arg1Ref->name, "", quadArgType::variable, arg1Ref};

    if(arg2Ref == nullptr) {
        this->arg2 = quadArg{};
    }
    else {
        this->arg2 = quadArg{0, nullptr, arg2Ref->name, "", quadArgType::variable, arg2Ref};
    }

    this->result = quadArg{0, nullptr, resultRef->name, "", quadArgType::variable, resultRef};
}

void Quad::ResetQuadCounters() {
    temporaryCounter = 1;
    CFLabelCounter = 1;
}

std::string Quad::GetNewTemporary() {
    return "_t" + std::to_string(Quad::temporaryCounter++);
}

/* Get new label for control flow statements in three-address code. */
std::string Quad::GetNewCFLabel() {
    return "_L" + std::to_string(Quad::CFLabelCounter++);
}

/* Takes in a source code operator (e.g. "+") and converts it to the corresponding
 * quad enum operator type. */
quadOpType Quad::SourceOptoQuadOp(std::string op, std::string type) {
    if(op == "*")        return type == "float" ? quadOpType::fmul : quadOpType::imul;
    else if(op == "/")   return type == "float" ? quadOpType::fdiv : quadOpType::idiv;
    else if(op == "%")   return quadOpType::imod;
    else if(op == "+")   return type == "float" ? quadOpType::fadd : quadOpType::iadd;
    else if(op == "-")   return type == "float" ? quadOpType::fminus : quadOpType::iminus;
    else if(op == "<")   return type == "float" ? quadOpType::flt : quadOpType::ilt;
    else if(op == "<=")  return type == "float" ? quadOpType::fleq : quadOpType::ileq;
    else if(op == "==")  return type == "float" ? quadOpType::feq : quadOpType::ieq;
    else if(op == "!=")  return type == "float" ? quadOpType::fneq : quadOpType::ineq;
    else if(op == ">=")  return type == "float" ? quadOpType::fgeq : quadOpType::igeq;
    else if(op == ">")   return type == "float" ? quadOpType::fgt : quadOpType::igt;
    else if(op == "&&")  return quadOpType::iand;
    else if(op == "||")  return quadOpType::ior;
    else {
        cout_str(std::string("error : unrecognized source operator in tac.cpp SourceOptoQuadOp(...), ")
        + "returning quadOpType::nop...");
        return quadOpType::nop;
    }
}

/* Overload the output stream operator to print quad operations types. */
std::ostream& operator<<(std::ostream& os, quadOpType qType) {
    switch (qType) {
        case quadOpType::iadd : return os << "+" ;
        case quadOpType::fadd : return os << "+" ;
        case quadOpType::iminus : return os << "-" ;
        case quadOpType::fminus : return os << "-" ;
        case quadOpType::iunmin : return os << "-" ;
        case quadOpType::iunnot : return os << "!" ;
        case quadOpType::funmin : return os << "-" ;
        case quadOpType::idiv : return os << "/" ;
        case quadOpType::fdiv : return os << "/" ;
        case quadOpType::imul : return os << "*" ;
        case quadOpType::fmul : return os << "*" ;
        case quadOpType::imod : return os << "%" ;
        case quadOpType::istore : return os << "" ;
        case quadOpType::fstore : return os << "" ;
        case quadOpType::param : return os << "param" ;
        case quadOpType::call : return os << "call" ;
        case quadOpType::funclabel : return os << "funclabel" ;
        case quadOpType::ireturn : return os << "ireturn" ;
        case quadOpType::freturn : return os << "freturn" ;
        case quadOpType::jmp : return os << "jmp";
        case quadOpType::jmplabel : return os << "jmplabel";
        case quadOpType::ilt : return os << "<";
        case quadOpType::flt : return os << "<";
        case quadOpType::ileq : return os << "<=";
        case quadOpType::fleq : return os << "<=";
        case quadOpType::ieq : return os << "==";
        case quadOpType::feq : return os << "==";
        case quadOpType::ineq : return os << "!=";
        case quadOpType::fneq : return os << "!=";
        case quadOpType::igeq : return os << ">=";
        case quadOpType::fgeq : return os << ">=";
        case quadOpType::igt : return os << ">";
        case quadOpType::fgt : return os << ">";
        case quadOpType::iand : return os << "&&";
        case quadOpType::fand : return os << "&&";
        case quadOpType::ior : return os << "||";
        case quadOpType::nop : return os << "NOP";
    };
    return os << "unrecognized quadOpType in tac.cpp op ostream operator overloading : " << (int)qType;
}

/* Overload the output stream operator to print quad argument types. */
std::ostream& operator<<(std::ostream& os, quadArgType qArgType) {
    switch (qArgType) {
        case quadArgType::empty : return os << "empty" ;
        case quadArgType::variable : return os << "variable" ;
        case quadArgType::intConst : return os << "intConst" ;
        case quadArgType::charConst : return os << "charConst" ;
        case quadArgType::floatConst : return os << "floatConst" ;
        case quadArgType::boolConst : return os << "boolConst" ;
        case quadArgType::label : return os << "label" ;
        case quadArgType::unaryOp : return os << "unaryOp" ;
    };
    return os << "unrecognized quadArgType in op ostream operator overloading : " << (int)qArgType;
}

/* Prints information about the quadruple. Mainly for debugging. */
void Quad::PrintQuad(QuadInstruc quad) {
    std::cout << "quadOp :" << quad.op << "\n";
    PrintQuadArg(quad.arg1, "arg1   ");
    PrintQuadArg(quad.arg2, "arg2   ");
    PrintQuadArg(quad.result, "result ");
}

/* Print information about a quadruple argument. Mainly for debugging.  */
void Quad::PrintQuadArg(quadArg qArg, std::string quadField) {
    bool b = qArg.nodeRef == nullptr;
    std::string nodeRefIsNullptr = (b) ? " ExprNodeRef is nullptr" : " ExprNodeRef is not nullptr";

    std::cout << quadField << ": " << "qArg.isConstant: " << qArg.isConstant
    << nodeRefIsNullptr << " name: " << qArg.name
    << " label: " << qArg.label << " quadArgType: " << qArg.type << "\n";
}

/* Print the quadruples in Quad format. Mainly for debugging.  */
void Quad::PrintQuadInstrucs(std::vector<QuadInstruc> instructions) {
    for(QuadInstruc &instr : instructions) {
        PrintQuadInstruc(instr);
    }
}

void Quad::PrintQuadInstruc(QuadInstruc instr) {
    std::cout << "    " << GetQuadStr(instr);
}

/* Convert a quad to its string representation. A bit ugly atm, needs reformatting. */
std::string Quad::GetQuadStr(QuadInstruc instr) {
    quadArg arg1 = instr.arg1;
    quadArg arg2 = instr.arg2;
    quadArg result = instr.result;

    if(instr.op == quadOpType::call) {
        if(result.name != "") {
            return result.name + " = call " + arg1.label + ", " + arg2.label + '\n';
        }
        return "call " + arg1.label + ", " + arg2.label + '\n';
    }
    else if(instr.op == quadOpType::param) {
        if(instr.arg1.label == "decl_param") {
            return arg1.name + " " + "decl_param" + '\n';
        }
        else {
            return arg1.name + " " + "param" + '\n';
        }
    }
    else if(instr.op == quadOpType::iunmin) {
        return result.name + " = " + "-" + arg1.name + '\n';
    }
    else if(instr.op == quadOpType::funmin) {
        return result.name + " = " + "-" + arg1.name + '\n';
    }
    else if(instr.op == quadOpType::iunnot) {
        return result.name + " = " + "!" + arg1.name + '\n';
    }
    else if(instr.op == quadOpType::jmp) {
        if(arg1.name != "") {
            return "if false " + arg1.name + " goto " + result.label + '\n';
        }
        else {
            return "goto " + result.label + '\n';
        }
    }
    else if(instr.op == quadOpType::jmplabel) {
        return result.label + '\n';
    }
    else if(instr.op == quadOpType::ireturn) {
        return "ireturn " + arg1.name + '\n';
    }

    std::stringstream ss;

    if(result.type == quadArgType::label) {
        ss << result.label << " ";
    }

    if(result.type == quadArgType::variable) {
        ss << result.name << " = ";
    }

    /* Printing the 1st argument of the quadruple.  */
    if(arg1.isConstant) {
        if(FloatConst *constant = dynamic_cast<FloatConst*>(arg1.nodeRef))
            ss << constant->value << " ";
        else
            ss << arg1.nodeRef->GetIntValue() << " ";
    }
    else {
         ss << arg1.name << " ";
    }

    ss << instr.op << " ";

    if(arg2.isConstant) {
        if(FloatConst *constant = dynamic_cast<FloatConst*>(arg1.nodeRef))
            ss << constant->value << " ";
        else
            ss << arg1.nodeRef->GetIntValue() << " ";
    }
    else {
        if(arg2.name == "") {
            ss << '\n';
        }
        else {
            ss << arg2.name << '\n';
        }
    }

    return ss.str();
}

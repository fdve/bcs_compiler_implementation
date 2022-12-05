/* Header file containing several declarations and enums for the implementation
 * of the Quadruple, a structure to represent three-address code. */

#include <sstream>
#include <vector>

/* The type of operation of the quadruple. Both operands are of type int
 * in an int operation, and both operands are of type float in a float operation. */
enum quadOpType : int {
    istore,
    fstore,
    iadd,
    fadd,
    iminus,
    fminus,
    iunmin,         // unary int minus (-)
    funmin,         // unary float minus (-10.0)
    iunnot,         // unary int not   (!)
    idiv,
    fdiv,
    imul,
    fmul,
    imod,
    call,
    ireturn,
    freturn,
    param,
    funclabel,
    ilt,            // (int)   <
    flt,            // (float) <
    ileq,           // (int)   <=
    fleq,           // (float) <=
    ieq,            // (int)   ==
    feq,            // (float) ==
    ineq,           // (int)   !=
    fneq,           // (float) !=
    igeq,           // (int)   >=
    fgeq,           // (float) >=
    igt,            // (int)   >
    fgt,            // (float) >
    iand,           // (int)   &&
    fand,           // (float) &&
    ior,            // (int)   ||
    jmp,
    jmplabel,
    nop            // do nothing
};

/* The type of the arg1, arg2, or result quad field. */
enum quadArgType : int {
    empty,          // empty field
    variable,       // a source program name or compiler temporary
    intConst,      
    charConst,
    floatConst,
    boolConst,
    label,          // conditional labels (and funcs to call labels?)
    unaryOp,
};

std::ostream& operator<<(std::ostream& os, quadOpType qType);
std::ostream& operator<<(std::ostream& os, quadArgType qArgType);

/* Represent the quadruple arguments as structs. */
struct quadArg {
    bool isConstant = false;
    Expression *nodeRef = nullptr;
    std::string name = "";          // Storing the name of a variable argument
    std::string label = "";
    quadArgType type = quadArgType::empty;
    symTableEntry *entryRef = nullptr;
};

/* A tac instruction, represented as a quadruple. */
class QuadInstruc {
    public :
        QuadInstruc(quadOpType op, quadArg arg1, quadArg arg2, quadArg result)
        : op(op), arg1(arg1), arg2(arg2), result(result) {}

        QuadInstruc(quadOpType op, symTableEntry *arg1Ref, symTableEntry *arg2Ref, symTableEntry *resultRef);

        quadOpType op;
        quadArg arg1;
        quadArg arg2;
        quadArg result;
        std::string comment;
};

/* Class providing helper function for the TAC (imnplemented as Quadruples)
 * production process. */
class Quad {
    public :
        static void ResetQuadCounters();
        static void PrintQuadInstrucs(std::vector<QuadInstruc> instructions);
        static void PrintQuadInstruc(QuadInstruc instruction);
        static std::string GetQuadStr(QuadInstruc instruction);
        static std::string GetNewTemporary();
        static std::string GetNewCFLabel();
        static quadOpType SourceOptoQuadOp(std::string op, std::string type);
        static void PrintQuad(QuadInstruc quad);
        static void PrintQuadArg(quadArg qArg, std::string quadField);
        static std::vector<QuadInstruc> QuadInstrucs;

    private :
        static int temporaryCounter;
        static int CFLabelCounter;
};

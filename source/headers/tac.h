/* - []  Header file for the three-address code (TAC) class. (And tac instr ??)
    maybe make this into a quadruple imeplemtation class,
    and the tac into anotehr class

*/
#include <sstream>
#include <vector>

/* The type of operation of the quadruple. Both operands are of type int
 * in an int operation, and both operands are of type float in a float operation. */
enum quadOpType : int {
    iload,          // -[] dont use this, should we use this for anything?
    fload,          // -[] dont use this, should we use this for anything?
    istore,         // -[] change name of this?
    fstore,         // -[] change name of this?


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
    nop,



    // [] or is not allowed?
    icond
};

std::ostream& operator<<(std::ostream& os, quadOpType qType);

/* The type of the arg1, arg2, or result quad field. */
enum quadArgType {
    empty,          // empty field
    variable,       // a source program name or compiler temporary
    intConst,       // - [] merge constants into 1 const?
    charConst,
    floatConst,
    boolConst,
    label,          // conditional labels (and funcs to call labels?)
    unaryOp,
};

std::ostream& operator<<(std::ostream& os, quadArgType qArgType);

symTableEntry* ConstantTACGen(Expression *expr);

/* Represent the quadruple arguments as structs. */
/* If isConstant is true, the value is a literal constant.
 * If isConstant is false, the value represents the key into the symbol table. */
 /* - [] What if the constant is a float / int ? */
 /* make class for consistency or? */
struct quadArg {
    bool isConstant = false;
    Expression *nodeRef = nullptr;
    // int symTableKey = 0;
    std::string name = "";          // Storing the name of a variable argument
    std::string label = "";
    quadArgType type = quadArgType::empty;
    symTableEntry *entryRef = nullptr;
};

/* A tac instruction, represented as a quadruple. */
/* - [] privatize? */
class TACInstruction {
    public :
        TACInstruction(quadOpType op, quadArg arg1, quadArg arg2, quadArg result)
        : op(op), arg1(arg1), arg2(arg2), result(result) {}

        TACInstruction(quadOpType op, symTableEntry *arg1Ref, symTableEntry *arg2Ref, symTableEntry *resultRef);

        quadOpType op;
        quadArg arg1;
        quadArg arg2;
        quadArg result;
        std::string comment;
};

class TAC {
    public :
        static void ResetTACCounters();
        static void PrintTACInstructions(std::vector<TACInstruction> instructions);
        static void PrintTACInstruction(TACInstruction instruction);
        static std::string GetQuadStr(TACInstruction instruction);

        /* -[] Use some kind of algorithm to reuse temporaryis efficiently?
           some algorithm in the dragon book? */
        static std::string GetNewTemporary();
        static std::string GetNewCFLabel();
        static quadOpType SourceOptoQuadOp(std::string op, std::string type);
        static void PrintQuad(TACInstruction quad);
        static void PrintQuadArg(quadArg qArg, std::string quadField);

        static std::vector<TACInstruction> tacInstructions;
    private :
        static int temporaryCounter;
        static int CFLabelCounter;
};

/* Generate TAC by traversing the AST. */
// bool GenerateTAC(Program *programNode);

bool GenerateTACBinOp(BitwiseOpExpr *binOpExpr);
bool GenerateTACExpr(Expression *expr);

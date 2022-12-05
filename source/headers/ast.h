#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

class Expression;
class FuncCall;
class FuncDecl;
class FuncDeclParam;
class Statement;
class Identifier;
class ASTVisitor;
class CFWhile;
class SymbolTable;
class symTableEntry;

struct YYLTYPE;

typedef std::vector<Expression *> exprList;
typedef std::vector<Statement *> stmntList;
typedef std::vector<FuncDecl *> FuncDeclList;
typedef std::vector<FuncDeclParam *> FuncDeclParamList;

YYLTYPE* newInfo(YYLTYPE *info);

/* Base type for the AST nodes. */
class ASTNode {
    public :
        virtual ~ASTNode();
        void VisitASTNode(ASTVisitor *visitor);
};

/* Contains all the functions of our program, and represents one program.
 * Constructing programs according to the parse rules, with optional functions
 * on the left side in the tree, mandatory start function somewhere in the middle,
 * and optional functions on the right hand side of the tree. */
class Program : public ASTNode {
    public :
        Program(FuncDeclList *lhs, FuncDecl *start_fnc, FuncDeclList *rhs);

    private :
        /* Our program node contains a reference to the start function declaration, and
        * multiple declarations to optional functions before and after the start function. */
        FuncDeclList *lhs;
        FuncDeclList *rhs;
        FuncDecl *start_fnc;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class FuncDecl : public ASTNode {
    public :
        FuncDecl(std::string type, Identifier *id, FuncDeclParamList *paramList, stmntList *fncBlock, YYLTYPE *info)
        : type(type), id(id), paramList(paramList), fncBlock(fncBlock), info(newInfo(info)){}

        FuncDecl(stmntList *fncBlock);

    private :
        std::string type;
        Identifier *id;
        FuncDeclParamList *paramList;
        stmntList *fncBlock;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class FuncDeclParam : public ASTNode {
    public :
        FuncDeclParam(std::string type, Identifier *id)
        : type(type), id(id) {}

    private :
        std::string type;
        Identifier *id;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class Scope;
};

class Statement : public ASTNode {
    public :
    private :
};

/* 0 or more statements enclosed in curly brackets. */
class FunctionBlock : public Statement {
    public :
        FunctionBlock(stmntList *stmnts)
        : stmnts(stmnts){}

        /* To access stmnts inside of bisons parser.y */
        stmntList *GetStmnts();
    private :
        stmntList *stmnts;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class VariableDef : public Statement {
    public :
        VariableDef(std::string type, Identifier *id, Expression *expr, YYLTYPE *info)
        : type(type), id(id), expr(expr), info(newInfo(info)){}

    private :
        std::string type;
        Identifier *id;
        Expression *expr;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class SymbolTable;
};

class Assignment : public Statement {
    public :
        Assignment(Identifier *id, std::string assignOp, Expression *expr, YYLTYPE *info);

    private :
        Identifier *id;
        std::string assignOp;
        Expression *expr;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class Desugar;
};

/* An expression with a semi-colon at the end is a statement. */
class ExprStmnt : public Statement {
    public :
        ExprStmnt(Expression *expr) : expr(expr){}

    private :
        Expression *expr;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class ReturnCall : public Statement {
    public :
        ReturnCall(Expression *expr, Identifier *id, YYLTYPE *info)
        : expr(expr), id(id), info(newInfo(info)){}

    private :
        Expression *expr;
        Identifier *id;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class CFIf : public Statement {
    public :
        CFIf(Expression *expr, stmntList *ifStmnts, stmntList *elseStmnts, YYLTYPE *info)
        : expr(expr), ifStmnts(ifStmnts), elseStmnts(elseStmnts), info(newInfo(info)){}

    private :
        Expression *expr;
        stmntList *ifStmnts;
        stmntList *elseStmnts;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class CFFor : public Statement {
    public :
        CFFor(Statement *varDef, Expression *m_expr, Statement *assign, stmntList *stmnts, YYLTYPE *info)
        : varDef(varDef), m_expr(m_expr), assign(assign), stmnts(stmnts), info(newInfo(info)){}

    private :
        Statement *varDef;
        Expression *m_expr;
        Statement *assign;
        stmntList *stmnts;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class Desugar;
};

class CFWhile : public Statement {
    public :
        CFWhile(Expression *expr, stmntList *stmnts, YYLTYPE *info)
        : expr(expr), stmnts(stmnts), info(newInfo(info)){}

    private :
        std::string beforeDesugar;
        Expression *expr;
        stmntList *stmnts;
        YYLTYPE *info;

        friend class Desugar;
        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
};

class CFDoWhile : public Statement {
    public :
        CFDoWhile(Expression *expr, stmntList *stmnts, YYLTYPE *info)
        : expr(expr), stmnts(stmnts), info(newInfo(info)){}

    private :
        Expression *expr;
        stmntList *stmnts;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class Desugar;
};

class Expression : public ASTNode {
    public :
        /* Define a virtual method for returning the type, so that during type verification,
           base class expressions can be compared for compatible types. */
        virtual std::string getType() { return "expression_base_type"; }

        /* Returns integer value in case of IntConst, BoolConst and CharConst nodes. */
        virtual int GetIntValue() { return 0; }
};

class UnaryOpExpr : public Expression {
    public :
        UnaryOpExpr(char unaryOp, Expression *expr, YYLTYPE *info)
        : unaryOp(unaryOp), expr(expr), info(newInfo(info)){}

        std::string getType() { return this->type; }

    private :
        char unaryOp;
        Expression *expr;
        std::string type;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class TypeResolution;
};

class BitwiseOpExpr : public Expression {
    public :
        BitwiseOpExpr(Expression *lExpr, std::string binOp, Expression *rExpr, YYLTYPE *info);
        std::string getType() { return this->type; }

    private :
        Expression *lExpr;
        std::string binOp;
        Expression *rExpr;
        std::string type;
        YYLTYPE *info;

        friend class TypeResolution;
        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class Desugar;
};

class FuncCall : public Expression {
    public :
        FuncCall(std::string type, Identifier *id, std::vector<Expression *> *params, YYLTYPE *info)
        : type(type), id(id), params(params), info(newInfo(info)){}

        std::string getType() { return this->type; }

    private :
        std::string type;
        Identifier *id;
        std::vector<Expression*> *params;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class SymbolTable;
        friend class TypeResolution;
};

class Identifier : public Expression {
    public :
        Identifier(std::string type, std::string name)
        : type(type), name(name){}

        Identifier(std::string type, std::string name, symTableEntry *symTableEntry)
        : type(type), name(name), entryRef(symTableEntry){}

        Identifier(symTableEntry *symTableEntry);

        Identifier(std::string name)
        : name(name){}

        Identifier(std::string name, YYLTYPE *info)
        : name(name), info(newInfo(info)){}

        std::string getType() { return this->type; }

    private :
        std::string type;
        std::string name;
        YYLTYPE *info;
        symTableEntry *entryRef;

        friend class Scope;
        friend class TypeResolution;
        friend class PrintVisitor;
        friend class SemanticsVisitor;
        friend class QuadGenVisitor;
        friend class SymbolTable;
};

class IntConst : public Expression {
    public :
        IntConst(int value) : value(value){}

        std::string getType() { return this->type; }
        int GetIntValue() { return value; }

    private :
        int value;
        std::string type = "int";

        friend class PrintVisitor;
        friend class QuadGenVisitor;
        friend class QuadInstruc;
        friend class ASMCodeGenerator;
};

class FloatConst : public Expression {
    public :
        FloatConst(float value) : value(value){}

        std::string getType() { return this->type; }
    private :
        float value;
        std::string type = "float";

        friend class PrintVisitor;
        friend class Quad;
        friend class QuadGenVisitor;
        friend class QuadInstruc;
        friend class ASMCodeGenerator;
        friend class SymbolTable;
};

class BoolConst : public Expression {
    public :
        BoolConst(bool value) : value(value){}

        std::string getType() { return this->type; }
        int GetIntValue() { return value; }

    private :
        std::string type = "bool";
        bool value;

        friend class PrintVisitor;
        friend class QuadGenVisitor;
        friend class QuadInstruc;
        friend class ASMCodeGenerator;
};

class CharConst : public Expression {
    public :
        CharConst(std::string value, YYLTYPE *info);

        std::string getType() { return this->type; }
        int GetIntValue() { return value; }

    private :
        bool CheckCharLegitimacy(std::string charString);
        int ConvertCharStrToInt(std::string charString);

        std::string type = "char";
        int value;
        YYLTYPE *info;

        friend class PrintVisitor;
        friend class QuadGenVisitor;
        friend class QuadInstruc;
        friend class ASMCodeGenerator;
};

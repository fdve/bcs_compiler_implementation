/* Several tree visitors are defined to execute various tasks
 * while traversing the AST. */

class symTableEntry;

/* Abstract base class serving as template for the deriving visitors. */
/* -[] change file name to astvisitors?? */
class ASTVisitor  {
    public :
        virtual ~ASTVisitor() {}

    protected:
        ASTVisitor(){}
};

/* Visitor class which prints the AST while traversing it. */
class PrintVisitor : public ASTVisitor {
    public:
        void visit(ASTNode *node, int depth);
        void visit(Program *node, int depth);
        void visit(FuncDecl *node, int depth);
        void visit(FuncDeclParam *node, int depth);
        void visit(Statement *node, int depth);
        void visit(VariableDef *node, int depth);
        void visit(Expression *node, int depth);
};

/* Type visitor class which annotates nodes with correct types, and checks
 * type compatibility. */
class TypeVisitor : public ASTVisitor {
    public:
        void visit(ASTNode *node);
        void visit(Program *node);
        void visit(FuncDecl *node);
        void visit(FuncDeclParam *node);
        void visit(Statement *node);
        void visit(VariableDef *node);
        void visit(Expression *node);

        /* - [] split these from type visitor? */
        void annotateExpr(Expression *node);
        void annotateBinOp(BitwiseOpExpr *node);
        void annotateUnaryOp(UnaryOpExpr *node);
};

class TACGenVisitor : public ASTVisitor {
    public:
        void visit(ASTNode *node);
        void visit(Program *node);
        void visit(FuncDecl *node);
        void visit(FuncDeclParam *node, bool lastParam);
        void visit(Statement *node);
        symTableEntry* visit(Expression *node);
};

class ASTVisitor;

class QuadGenVisitor : public ASTVisitor {
    public:
        void visit(ASTNode *node);
        void visit(Program *node);
        void visit(FuncDecl *node);
        void visit(FuncDeclParam *node, bool lastParam);
        void visit(Statement *node);
        symTableEntry* visit(Expression *node);
        static symTableEntry* ConstantQuadGen(Expression *expr);
};

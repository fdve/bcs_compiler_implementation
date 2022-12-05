class ASTVisitor;

/* Semantic analysis visitor class. */
class SemanticsVisitor : public ASTVisitor {
    public:
        void visit(ASTNode *node);
        void visit(Program *node);
        void visit(FuncDecl *node);
        void visit(FuncDeclParam *node);
        void visit(Statement *node);
        void visit(VariableDef *node);
        void visit(Expression *node);
};

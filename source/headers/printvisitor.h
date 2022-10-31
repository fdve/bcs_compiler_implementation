class ASTVisitor;

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

class ASTVisitor;

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
};

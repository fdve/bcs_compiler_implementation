#include <string>

class ASTVisitor;

/* Visitor class which prints the AST while traversing it. */
class PrintVisitor : public ASTVisitor {
    public:
        void visit(ASTNode *node);
        void visit(Program *node);
        void visit(FuncDecl *node);
        void visit(Statement *node);
        void visit(VariableDef *node);
        void visit(Expression *node);
};

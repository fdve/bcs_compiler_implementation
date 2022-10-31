/* Abstract base class serving as template for the deriving tree visitors. */
class ASTVisitor  {
    public :
        virtual ~ASTVisitor() {}

    protected:
        ASTVisitor(){}
};

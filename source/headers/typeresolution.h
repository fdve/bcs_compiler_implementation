/* The header file which contains helper functions for type compatiblity checks,
 * and annotating expression nodes with their correct types. */

#include <string>

struct YYLTYPE;

class TypeResolution {
    public:
        static void IncompatibleTypes(std::string type1, std::string type2);
        static void IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string idName, std::string type1, std::string type2);
        static void IncompatibleTypesBinOp(YYLTYPE *info, std::string name, Expression *exprLhs, Expression *exprRhs);
        static void annotateExpr(Expression *expr);
        static void annotateBinOp(BitwiseOpExpr *node);
        static void annotateUnaryOp(UnaryOpExpr *node);
};

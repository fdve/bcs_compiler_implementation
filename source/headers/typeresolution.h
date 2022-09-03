/* The header file which contains helper functions for type compatiblity checks.

   (1) Operations between types should be defined,
   (2) and only correct types should be assigned to variables.

   (1) Operations between types depend on the operator at hand.
       An equality operator may have different legimate operand
       types than an arithmetic operator.

   */

#include <string>

struct YYLTYPE;

class TypeResolution {
    public:
        static void IncompatibleTypes(std::string type1, std::string type2);
        static void IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string idName, std::string type1, std::string type2);
        static void IncompatibleTypesBinOp(YYLTYPE *info, std::string name, Expression *exprLhs, Expression *exprRhs);
};

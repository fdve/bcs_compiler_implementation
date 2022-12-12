/* Helper class for printing the AST in a pretty way. */

#include <vector>
#include <string>

class Expression;

class PrettyTree {
    public:
        static int currStmntDepth;
        static bool visitCFCond;

        /* For every nested statement, maintain a vector whether
         * the current nested statement which is being traversed
         * is the last statement of the list of statements in which this statement resides */
        static std::vector<bool> lastNestedStmnts;

        /* for current expression depth, maintain vector of left and
         * right subtree turns which where made to get to this depth,
         * where right represents true and
         * left subtree represents false */
        static std::vector<bool> currExprRSubtree;

        static bool lastFuncDec;
        static bool lastRootStmnt;

        /* Last statement on this depth */
        static bool lastDepthStmnt;
        static bool noStmntsCF;
        static bool lastFncCallArg;
        static bool inFncCallArgExpr;

        static void InitPrettyTree();
        static std::string StmntRoots();
        static std::string ExprRoots();
        static std::string NestedStmntBranches();
        static std::string NestedExprBranches(Expression *expr);
        static std::string FncDecBranches();
        static std::string AllStmntBranches();
        static std::string AllExprBranches(Expression *expr);
};

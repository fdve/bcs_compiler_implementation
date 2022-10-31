/* Class which desugars statments like k -= 1 to k = k - 1; and
 * translates for loops to while loops with equivalent functionality. */

#include <unordered_map>

class Desugar {
    public :
        static int FindModifier(std::string mod);
        static void DesugarAssignModifier(Assignment *assignNode, Expression *lExpr,
            std::string op, Expression *rExpr, YYLTYPE *info );

        /* Desugars modifier assignment nodes like i += 2 into a i = i + 2 node. */
        static bool DesugarModifier(Assignment *assignNode) ;
        static CFWhile* DesugarDoWhile(stmntList *smntsList, CFDoWhile *doWhileNode);
        static CFWhile *DesugarFor(stmntList *smntsList, CFFor *forNode);


    private :
        static const std::unordered_map<std::string, int> modifiers;
};

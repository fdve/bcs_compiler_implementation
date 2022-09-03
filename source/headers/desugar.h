/* Class which desugars statments like k -= 1 to k = k - 1; and
 * translates for loops to while loops with equivalent functionality. */

int findModifier(std::string mod);

void DesugarAssignModifier(Assignment *assignNode, Expression *lExpr,  std::string op, Expression *rExpr, YYLTYPE *info );



/* Desugars nodes like i += 2 into a i = i + 2 node. */
bool desugarModifier(Assignment *assignNode) ;

CFWhile* desugarDoWhile(stmntList *smntsList, CFDoWhile *doWhileNode);

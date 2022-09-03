/* Optimize the TAC. */

#include <utility>
#include <vector>
#include <string>

#include "../headers/ast.h"
#include "../headers/util.h"
#include "../headers/tac.h"

/* Remove all code after a return instruction, since that will be unreachable
 * and thus dead code. Stop when a new function label is encountered.  */
void RemoveAllCodeAfterRet(std::vector<TACInstruction> &instructions) {
    for (std::vector<TACInstruction>::iterator it = instructions.begin(); it != instructions.end();) {
        if((*it).op == quadOpType::ireturn) {
            bool skipFirstReturn = true;

            while(it != instructions.end()) {
                if(skipFirstReturn) {
                    skipFirstReturn = false;
                    ++it;
                    continue;
                }

                if((*it).op == quadOpType::funclabel) {
                    break;
                }
                it = instructions.erase(it);
            }
        }
        else {
            ++it;
        }
    }
}

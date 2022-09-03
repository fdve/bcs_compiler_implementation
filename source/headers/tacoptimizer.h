/* Optimize the TAC. */

#include <vector>

class TACInstruction;

void RemoveAllCodeAfterRet(std::vector<TACInstruction> &instructions);

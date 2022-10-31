/* Symbol table is a class which handles the programs encountered symbols (variable ID's,
 * function ID's) with helper functions. */

#include <utility>
#include <unordered_map>

class FuncDeclParam;
class Scope;
class Expression;

struct YYLTYPE;

typedef std::vector<FuncDeclParam *> FuncDeclParamList;

struct symTableEntry {
    std::string type = "";
    std::string name = "";
    Expression *nodeRef = nullptr;

    /* Store where the variable is saved on the asm stack. */
    std::string ASMAddress = "";

    /* Store the parameter types of a function symbol. */
    FuncDeclParamList *params = nullptr;

    /* Store information about on what line the symbol is declared and more.*/
    YYLTYPE *info;
};

class SymbolTable {
    public :
        symTableEntry* GetSymbol(std::string key);

        static void PrintTable(std::unordered_map<std::string, symTableEntry> hm, int scopeNum);
        static void PrintEntry(symTableEntry entry, int scopeNum);

        /* The symbol table for compiler temporaries is independant of scopes,
         * so declare one static temporaries symbol table here. This also avoids
         * name collisions between a temporary called _t1 and a scope variable called _t1. */
        static std::unordered_map<std::string, symTableEntry> temporariesSymTab;

    private :
        std::unordered_map<std::string, symTableEntry> hashmap;

        friend class Scope;
};

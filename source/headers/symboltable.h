/* Symbol table is a class which handles the programs encountered symbols (variable ID's,
   function ID's) with helper functions and stores them in the appropriate scope. */

#include <utility>
#include <unordered_map>

class FuncDeclParam;
class FuncCall;
class Scope;
class Statement;
class Identifier;
class VariableDef;
class Expression;

struct YYLTYPE;

typedef std::vector<FuncDeclParam *> funcDeclParamList;
typedef std::pair <std::string, std::string> symbol;
typedef std::pair <std::string, std::vector<std::string>> func_symbol;

/* - []  Need to add the value here? */
struct symTableEntry {
    /* - [] Make enum of type? */
    std::string type = "";
    std::string name = "";

     /* - [] remove line def? all stored info is ininfo*/
    int line_def = 0;

    Expression *nodeRef = nullptr;

    /* Store where the variable is saved on the asm stack. */
    std::string ASMAddress = "";

    /* Store the parameter typs  of a function symbol. */
    funcDeclParamList *params = nullptr;

    /* Store information about on what line the symbol is declared and more.*/
    YYLTYPE *info;
};

class SymbolTable {
    public :
        symTableEntry* GetSymbol(std::string key);

        /* -[] make PrintTable not static?  */
        static void PrintTable(std::unordered_map<std::string, symTableEntry> hm, int scopeNum);
        static void PrintEntry(symTableEntry entry, int scopeNum);

        /* The symbol table for compiler temporaries is independant of scopes,
         * so declare the table seperately here. */
        static std::unordered_map<std::string, symTableEntry> temporariesSymTab;

    private :
        std::unordered_map<std::string, symTableEntry> hashmap;

        friend class Scope;
};

class Scope {
    public :
        Scope();
        static void ClearScopes();

        static void PushScope();
        static void PopScope();
        static bool ContainsKey(std::string key);
        static symTableEntry* AddSymbol(symTableEntry entry, YYLTYPE *info);
        static symTableEntry* AddSymbol(std::string name, YYLTYPE *info);
        static symTableEntry* AddSymbol(std::string type, std::string name, YYLTYPE *info);
        static void addFuncParams(funcDeclParamList* params, YYLTYPE *info);
        static symTableEntry* GetSymbol(std::string name);
        static bool SymbolIsFunc(std::string key);
        static void PrintScopes();

    private :
        static void PrintScopes(Scope *scope, int depth);

        /* Store scopes children to enable printing of the whole tree from the root. */
        std::vector<Scope *> children;

        /* Representing a traversable "stack" from the root scope to the current scope. */
        static std::vector<Scope *> scopes;

        static Scope *currentScope;
        static Scope *globalScope;

        /* Every scope has its own symbol table. */
        SymbolTable symbolTable;

        friend class SymbolTable;
};

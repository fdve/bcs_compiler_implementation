/* Header for the scope class, in which current identifiers alive in the current
 * or higher score are being stored, while also saving the whole scope tree
 * with for every scope its symbol table, so that at a later point symbols
 * can still be referenced, for e.g. getting their values. */

#include <utility>
#include <unordered_map>

class FuncDeclParam;
class Scope;
class Expression;
class SymbolTable;

struct YYLTYPE;

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
        static void addFuncParams(FuncDeclParamList* params, YYLTYPE *info);
        static symTableEntry* GetSymbol(std::string name);
        static bool SymbolIsFunc(std::string key);
        static void PrintScopes();

    private :
        static void PrintScopes(Scope *scope, int depth);

        /* Store scopes children to enable printing of the whole scope tree from the root. */
        std::vector<Scope *> children;

        /* Representing a traversable "stack" from the root scope to the current scope. */
        static std::vector<Scope *> scopes;

        static Scope *currentScope;
        static Scope *globalScope;

        /* Every scope has its own symbol table. */
        SymbolTable symbolTable;

        friend class SymbolTable;
};

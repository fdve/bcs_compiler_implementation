#include <string>
#include <vector>
#include <utility>

#include "../headers/symboltable.h"
#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/errorlogger.h"

std::unordered_map<std::string, symTableEntry> SymbolTable::temporariesSymTab;


Scope::Scope() {}
Scope* Scope::currentScope = nullptr;
Scope* Scope::globalScope = nullptr;
std::vector<Scope *> Scope::scopes;

void Scope::ClearScopes() {
    Scope::scopes.clear();
    Scope::currentScope = nullptr;
    Scope::globalScope = nullptr;
}

void Scope::PushScope() {
    // cout_str("push_scope called");
    Scope *scope = new Scope();

    if(Scope::currentScope == nullptr) {
        /* -[] If scope is nullptr, new file is being read. Since scopes is static,
         * and files can be feeded one after another, reset the scopes here to
         * reset the sym tables. */
        // scopes.clear();

        /* This also resets the static globalScope which might had a reference to a previous
         scope from previous file? */


        Scope::currentScope = scope;
        Scope::globalScope = scope;
    }
    else {
        Scope::currentScope->children.push_back(scope);
        Scope::currentScope = scope;
    }

    Scope::scopes.push_back(scope);
}

void Scope::PopScope() {
    // cout_str("pop_scope called");
    Scope::scopes.pop_back();

    /* If all scopes popped,reset the value of the current scope back to a nullptr. */
    if(Scope::scopes.size() == 0) {
        // cout_str("Scope::PopScope(), scopes vec size is 0, assigning nullptr to curr and glob Scope");
        Scope::currentScope = nullptr;
        return;
    }

    Scope::currentScope = Scope::scopes.back();
    // std::cout << "in popScope, vec size = " << Scope::scopes.size() << '\n';
}

/* Check whether the current or parent scopes contain a symTableEntry.
 * Return true if found, otherwise return false and report error. */
bool Scope::ContainsKey(std::string key) {
    for(int i = Scope::scopes.size() - 1; i >= 0; i--) {
        auto *currentSymTab = &Scope::scopes.at(i)->symbolTable.hashmap;
        auto entry = currentSymTab->find(key);

        if (entry != currentSymTab->end()) {
            // cout_str("Scope::ContainsKey : found key " + key + " entry in hashmap");
            return true;
            // return &(entry->second);
        }
    }
    // print error here? throw? push error to logger?

    /*-[]  Only report this error when a variable is referenced, not when it is defined / declared. */
    // ErrorLogger::InsertMsg(ErrorLogger::UndefinedVar(*$1));


    return false;
}

/* Check whether symbol is a function (it is if it is in the global scope. )*/
bool Scope::SymbolIsFunc(std::string key) {
    auto currentSymTab = &Scope::globalScope->symbolTable;
    symTableEntry* entry = currentSymTab->GetSymbol(key);
    return entry != nullptr;
}


/* Traverse current scope up to root scope, trying to find entry.
 * If found, return ref to entry. Otherwise, return nullptr. */
symTableEntry* Scope::GetSymbol(std::string key) {
    // cout_str(std::to_string(Scope::scopes.size()));

    for(int i = Scope::scopes.size() - 1; i >= 0; i--) {
        auto currentSymTab = &Scope::scopes.at(i)->symbolTable;
        // std::cout << "Scope::GetSymbol(" << key << "), addr of currentSymTab : " << &currentSymTab << '\n';


        symTableEntry* entry = currentSymTab->GetSymbol(key);



        if(entry != nullptr) {
            // cout_str("Scope::getSymbol : found entry with key " + key);
            // std::cout << "Scope::getSymbol : entry address : " << entry << std::endl;
            return entry;
        }
    }
    // cout_str("Scope::getSymbol : did not find entry with key " + key + " , returning nullptr");
    return nullptr;
}




/* Return entry ref on successfull adding attempt, return nullptr otherwise. */
symTableEntry* Scope::AddSymbol(symTableEntry entry, YYLTYPE *info) {
    if(Scope::ContainsKey(entry.name)) {
        ErrorLogger::InsertMsg(ErrorLogger::RedefineVar(newInfo(info), entry.name));

        // ErrorLogger::InsertMsg(ErrorLogger::RedefineVar(&@1, *$1));

        return nullptr;
    }

    entry.info = newInfo(info);
    currentScope->symbolTable.hashmap[entry.name] = entry;
    // std::cout << "Scope::AddSymbol " << entry.name << " at address " << &currentScope->symbolTable.hashmap[entry.name] << std::endl;
    return &currentScope->symbolTable.hashmap[entry.name];
}

/* Add a function symbol.
 * Return entry ref on successfull adding attempt, return nullptr otherwise. */
// symTableEntry* Scope::AddSymbol(std::string type, std::string name, funcDeclParamList* params) {
//     symTableEntry entry;
//     entry.type = type;
//     entry.name = name;
//     entry->funcParams = params;
//     return AddSymbol(entry);
// }

/* Return entry ref on successfull adding attempt, return nullptr otherwise.
 * Enter a symtable entry, and set its name. */
symTableEntry* Scope::AddSymbol(std::string name, YYLTYPE *info) {
    symTableEntry entry;
    entry.name = name;
    entry.info = info;
    return AddSymbol(entry, info);
}

/* Return entry ref on successfull adding attempt, return nullptr otherwise.
 * Enter a symtable entry, and set its name and type. */
symTableEntry* Scope::AddSymbol(std::string type, std::string name, YYLTYPE *info) {
    symTableEntry entry;
    entry.type = type;
    entry.name = name;
    entry.info = info;
    return AddSymbol(entry, info);
}


// [] what happens if i throw exception from here?
void Scope::addFuncParams(funcDeclParamList* params, YYLTYPE *info) {
   for(FuncDeclParam *param : *params) {
       // cout_str("this->addVar(param->id->name) = " + (param->id->name));

       symTableEntry entry;
       entry.type = param->id->type;
       entry.name = param->id->name;
       entry.info = info;

       currentScope->AddSymbol(entry, info);
   }
}

/* -[] Variables in the Start function are indented different than childrren scopes of
 * not start functions, probably because of that. */

 /* Prints given scope and all of its children contents recursively. */
void Scope::PrintScopes() {
    PrintScopes(Scope::globalScope, 0);
}

void Scope::PrintScopes(Scope *scope, int depth) {
    if(scope == nullptr) {
        return;
    }

    SymbolTable::PrintTable(scope->symbolTable.hashmap, depth);

    for(Scope *scope : scope->children) {
        PrintScopes(scope, depth + 1);
    }
}

/* - [] add a getfunc symbol func? */
symTableEntry* SymbolTable::GetSymbol(std::string key) {
    // std::cout << "SymbolTable::GetSymbol, address this->hashmap : " << &this->hashmap << " key: " << key << '\n';

    auto entry = this->hashmap.find(key);
    return (entry != this->hashmap.end()) ? &(entry->second) : nullptr;
}

void SymbolTable::PrintTable(std::unordered_map<std::string, symTableEntry> hm, int depth) {
    // Printing symbol table (order is type, name, line_def, constNodeRef)

    for (std::pair<std::string, symTableEntry> const &keyValPair: hm) {
        // std::cout << &keyValPair.second << " ";
        PrintEntry(keyValPair.second, depth);
    }
}

/* -[] instead of cout, return the output string here?
 then you can maybe remove the scopeNUm arg etc ? */

 /* -[] make entry nullptr? */
void SymbolTable::PrintEntry(symTableEntry entry, int depth) {
    std::cout << spaced_str(depth * 2) << "scope " << depth << ": " << entry.type
    << " " << entry.name << " " << entry.line_def << " " ;



    /* If there is a reference to a constant node, it means the entry has a
     * value associated with it on createn (e.g. int a = 10;). Print that value. */
    /* - [] make prettier? */
    if(entry.nodeRef != nullptr) {
        if(FloatConst *constant = dynamic_cast<FloatConst*>(entry.nodeRef))
            std::cout << constant->value << "      ";
        else
            std::cout << entry.nodeRef->GetIntValue() << "      ";
    }
    else {
        std::cout << "nullptr ";
    }

    std::cout << entry.ASMAddress << std::endl;
}

#include <string>
#include <vector>
#include <utility>

#include "../headers/symboltable.h"
#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/scope.h"
#include "../headers/errorlogger.h"

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
    Scope *scope = new Scope();

    if(Scope::currentScope == nullptr) {
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
    Scope::scopes.pop_back();

    /* If all scopes popped,reset the value of the current scope back to a nullptr. */
    if(Scope::scopes.size() == 0) {
        Scope::currentScope = nullptr;
        return;
    }

    Scope::currentScope = Scope::scopes.back();
}

/* Check whether the current or parent scopes contain a symTableEntry.
 * Return true if found, otherwise return false and report error. */
bool Scope::ContainsKey(std::string key) {
    for(int i = Scope::scopes.size() - 1; i >= 0; i--) {
        auto *currentSymTab = &Scope::scopes.at(i)->symbolTable.hashmap;
        auto entry = currentSymTab->find(key);

        if (entry != currentSymTab->end()) {
            return true;
        }
    }

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
    for(int i = Scope::scopes.size() - 1; i >= 0; i--) {
        auto currentSymTab = &Scope::scopes.at(i)->symbolTable;
        symTableEntry* entry = currentSymTab->GetSymbol(key);

        if(entry != nullptr) {
            return entry;
        }
    }
    return nullptr;
}

/* Return entry ref on successfull adding attempt, return nullptr otherwise. */
symTableEntry* Scope::AddSymbol(symTableEntry entry, YYLTYPE *info) {
    if(Scope::ContainsKey(entry.name)) {
        ErrLog::InsertMsg(ErrLog::RedefineVar(newInfo(info), entry.name));
        return nullptr;
    }

    entry.info = newInfo(info);
    currentScope->symbolTable.hashmap[entry.name] = entry;
    return &currentScope->symbolTable.hashmap[entry.name];
}

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


void Scope::addFuncParams(FuncDeclParamList* params, YYLTYPE *info) {
   for(FuncDeclParam *param : *params) {
       symTableEntry entry;
       entry.type = param->id->type;
       entry.name = param->id->name;
       entry.info = info;
       currentScope->AddSymbol(entry, info);
   }
}

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

#include <string>
#include <vector>
#include <utility>

#include "../headers/symboltable.h"
#include "../headers/scope.h"
#include "../headers/util.h"
#include "../headers/ast.h"
#include "../headers/errorlogger.h"
#include "../headers/parser.hpp"

std::unordered_map<std::string, symTableEntry> SymbolTable::temporariesSymTab;

symTableEntry* SymbolTable::GetSymbol(std::string key) {
    auto entry = this->hashmap.find(key);
    return (entry != this->hashmap.end()) ? &(entry->second) : nullptr;
}

void SymbolTable::PrintTable(std::unordered_map<std::string, symTableEntry> hm, int depth) {
    /* Printing symbol table (order is type, name, line_def, constNodeRef) */
    for (std::pair<std::string, symTableEntry> const &keyValPair: hm) {
        PrintEntry(keyValPair.second, depth);
    }
}

void SymbolTable::PrintEntry(symTableEntry entry, int depth) {
    std::cout << spaced_str(depth * 2) << "scope " << depth << ": " << entry.type
    << " " << entry.name << " " << entry.info->first_line << " " ;

    /* If there is a reference to a constant node, it means the entry has a
     * value associated with it on creation (e.g. int a = 10;). Print that value. */
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

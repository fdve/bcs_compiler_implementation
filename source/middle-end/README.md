quadgenvisitor.cpp is the core of the middle-end. All the nodes of the abstract syntax tree are visited and appropriate three-address code is generated in the form of a quadruple. Symbol table entries are also created if needed.

quad.cpp takes care of creating quadruples, getting new compiler temporaries, getting three-address code lables for instructions to jump to, converting operators found in abstract syntax tree nodes into the corresponding three-address code operation, as well as printing quadruples which is needed for debugging and for printing when the -tac flag is provided.

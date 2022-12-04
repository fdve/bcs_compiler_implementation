The front-end consists of classes for creating and managing an abstract syntax tree, for desugaring statements, for lexing the input file, parsing the lexemes, printing of the ast, for scopes, for comparing types and annotating expressions with their types, and finally for semantic analysis.

To see the definition of the abstract syntax tree look at the ast.h file in /source/headers/ast.h.

Desugaring is implemented in the desugar.cpp file, and functions from desugar.cpp are called both in the Bison parser.y file as well as ast.cpp.

Lexing is implemented in the Flex file lexer.l. A YY_USER_ACTION is defined in order to keep track of the location of the lexeme in the source program.

Parsing is implemented in the Bison file parser.y. The type of terminals and non terminals are defined, as well as precedence rules. The parser file constructs the abstract syntax tree by parsing the lexemes.

Printing of the abstract syntax tree is implemented in the printvisitor.cpp file. It uses the Visitor design pattern and prints the contents of every encountered node.

Scope management is implemented in scope.cpp. During runtime, scopes are being stored in a stack of scopes and their references are saved, so that after the front-end compilation phase all the scopes and their contents can still be printed out for debugging purposes.

Type resolution and type comparison is implemented in typeresolution.cpp. Expressions can take the form of tree structures, which can make it difficult to determine the types of every expression node in such a tree on creation. This is why a seperate pass over expressions is made to annotate the expression nodes with the correct types.

Semantic analysis is implemented in typevisitor.cpp. The TypeVisitor class traverse the abstract syntax tree and makes sure called functions are being called with the correct amount of arguments, that types of binary operations are compatiable, and so on.

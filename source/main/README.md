compile_source_file.cpp tries to compile an input file up until the provided compiler phase. Also, the optional interface options such as printing the abstract syntax tree as well as printing the three-address code are executed if set. compile_source_file.cpp is the file which brings all the different components of the compiler together.

compile_main.cpp parses the command line arguments and if valid, will pass the given input file to CompileSourceFile(...) in compile_source_file.cpp

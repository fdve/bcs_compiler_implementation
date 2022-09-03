extern int yyparse();
extern FILE *yyin;

int parse_file(const char *filePath);

int CompileSourceFile(int argc, std::string fileName, int compilePhase);

int CompileSourceFile(int argc, std::string inputFile, std::string outputFile, int compilePhase);

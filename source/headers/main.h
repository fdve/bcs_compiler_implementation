extern int yyparse();
extern FILE *yyin;

struct compFlags {
    std::string fileDest = "";
    bool invalidFlag = 0;
    bool customFileDest = 0;
    bool printAst = 0;
    bool printTac = 0;
};

int parse_file(const char *filePath);

int CompileSourceFile(int argc, std::string fileName, int compilePhase);

int CompileSourceFile(int argc, std::string inputFile, int compilePhase, compFlags flags);

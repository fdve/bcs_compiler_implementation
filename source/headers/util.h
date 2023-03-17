/* Header containing utility functions for the compiler. */

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class ASTNode;

struct YYLTYPE;
struct compFlags;

/* Either compile every phase (0), or only the front-end (1),
 * or the front and middle end (2). */
enum CompilePhase {
    all,
    front,
    middle
};

static const std::string cmmndLineArgsUsage = std::string("usage = ./star_compiler ([HELP_FLAG] | FILE_NAME [FLAGS])")
+ "\n--d=destination_path to define the path"
+ " for the compiled file\n-ast to print the ast generated during compilation"
+ "\n-tac to print the generated three-address code"
+ "\n--help to print this message";


YYLTYPE* newInfo(YYLTYPE *info);

void printInfo(YYLTYPE *info);

/* Check if certain pointer is a null pointer and return true if it is. */
bool check_nullptr(void *vp, std::string type);

/* Check if certain pointer is a nullptr and if so throws an exception. */
void check_nullptr_throws(void *vp, std::string type);

void EntryNullPtrMsg(std::string loc, std::string name);

void reverse_vector(std::vector<ASTNode*> *vec);

void cout_str(std::string msg);

std::string spaced_str(int spaces);

std::string repeat_str(std::string str, int n);

bool checkCorrectFileExt(std::string fileName);

compFlags ParseCmmndLineArgs(int argc, char *argv[]);

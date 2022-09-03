/* Header containing utility functions for the compiler. */

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class ASTNode;

struct YYLTYPE;


/* Either compile every phase (0), or only the front-end (1),
 * or up to the middle-end (2). */
enum CompilePhase {
    all,
    front,
    middle
};


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

bool checkCorrectFileExt(std::string fileName);

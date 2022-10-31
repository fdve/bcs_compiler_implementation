/* Header file for the error logger, which contains an inner class Errmsg
 * which represents the error messages. */

#include <sstream>
#include <string>
#include <vector>

class Expression;

struct YYLTYPE;

class ErrLog {
    public :
        class ErrMsg {
            public :
                ErrMsg(YYLTYPE *info, std::string msg)
                : info(info), msg(msg){}

                void Print();
                std::string Str();

                YYLTYPE *info;
                std::string msg;
        };

        static void Reset();
        static void PrintErrors();
        static std::string FormatType(std::string type);
        static std::string ErrorInfoString(YYLTYPE *info);
        static YYLTYPE* CreateInfo(int firstLine, int firstColumn);
        static void InsertMsg(ErrMsg msg);
        static ErrMsg ParserError(int line, int column, std::string errorMsg);
        static ErrMsg UndefinedFnc(YYLTYPE *info, std::string fnc);
        static ErrMsg UndefinedVar(YYLTYPE *info, std::string var);
        static ErrMsg RedefineVar(YYLTYPE *info, std::string var);
        static ErrMsg IllegitimateChar(YYLTYPE *info, std::string value);
        static ErrMsg IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string name, std::string type1, std::string type2);
        static ErrMsg IncompatibleTypesBinOps(YYLTYPE *info, std::string binop, std::string type1, std::string type2);
        static ErrMsg NonVoidFncNoReturn(YYLTYPE *info, std::string funcName);
        static ErrMsg ReturnError(YYLTYPE *info, std::string functionName, std::string returnType1, std::string returnType2);
        static ErrMsg InvalidNumParams(YYLTYPE *info, std::string functionName, int given, int expected);
        static ErrMsg InvalidTypesParams(YYLTYPE *info, std::string functionName, std::string type1, std::string type2);
        static ErrMsg InvalidTypesParams(YYLTYPE *info, std::string functionName, int paramNum, std::string expected, std::string given);
        static ErrMsg CalledSymbolIsNotFnc(YYLTYPE *info, std::string functionName);
        static ErrMsg FncDecTooManyParams(YYLTYPE *info, std::string functionName);

        static std::vector<ErrMsg> errorMsgs;
        static std::string fileName;
        static bool parsingError;

        /* Trims file to only contain the file name and no path. */
        static bool trimFilePath;
        static bool colorizeOutput;
};

bool operator==(ErrLog::ErrMsg lhs, ErrLog::ErrMsg rhs);
bool operator<(ErrLog::ErrMsg lhs, ErrLog::ErrMsg rhs);

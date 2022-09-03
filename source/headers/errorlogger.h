#include <sstream>
#include <string>
#include <vector>

class Expression;

struct YYLTYPE;

class ErrorLogger {
    public :
        class ErrorMsg {
            public :
                ErrorMsg(YYLTYPE *info, std::string msg)
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
        static void InsertMsg(ErrorMsg msg);
        static ErrorMsg ParserError(int lineo, std::string errorMsg);
        static ErrorMsg UndefinedFnc(YYLTYPE *info, std::string fnc);
        static ErrorMsg UndefinedVar(YYLTYPE *info, std::string var);
        static ErrorMsg RedefineVar(YYLTYPE *info, std::string var);
        static ErrorMsg IllegitimateChar(std::string value);
        static ErrorMsg IncompatibleTypes(YYLTYPE *info, std::string nodeName, std::string name, std::string type1, std::string type2);
        static ErrorMsg IncompatibleTypesBinOps(YYLTYPE *info, std::string binop, std::string type1, std::string type2);
        static ErrorMsg NonVoidFncNoReturn(YYLTYPE *info, std::string funcName);
        static ErrorMsg ReturnError(YYLTYPE *info, std::string functionName, std::string returnType1, std::string returnType2);
        static ErrorMsg InvalidNumParams(YYLTYPE *info, std::string functionName, int given, int expected);
        static ErrorMsg InvalidTypesParams(YYLTYPE *info, std::string functionName, std::string type1, std::string type2);
        static ErrorMsg InvalidTypesParams(YYLTYPE *info, std::string functionName, int paramNum, std::string expected, std::string given);
        static ErrorMsg CalledSymbolIsNotFnc(YYLTYPE *info, std::string functionName);
        static ErrorMsg FncDecTooManyParams(YYLTYPE *info, std::string functionName);

        static std::vector<ErrorMsg> errorMsgs;
        static std::string fileName;
        static bool parsingError;

        /* Trims file to only contain the file name and no path. */
        static bool trimFilePath;
        static bool colorizeOutput;
};

bool operator==(ErrorLogger::ErrorMsg lhs, ErrorLogger::ErrorMsg rhs);
bool operator<(ErrorLogger::ErrorMsg lhs, ErrorLogger::ErrorMsg rhs);

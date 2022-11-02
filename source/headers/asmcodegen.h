/* Class containing helper functions and constant strings for the
 * assembly code generating phase. */

enum quadOpType : int;

struct compFlags;

class QuadInstruc;

#include <vector>
#include <utility>
#include <unordered_map>

class ASMCodeGenerator {
    public :
        static void SetEntryPoint(std::string entryPointStr);
        static const std::string defaultFilePath;
        static bool setMainLabelToEntry;

    private :
        static int prevSpawnOffset;
        static int localConstCounter;
        static std::string x86_64_ENTRY_POINT;
        static const std::string x86_64_FUNC_INTRO;
        static const std::string x86_64_FUNC_OUTRO;

        static const std::unordered_map<std::string, int> typeToBytes;
        static const std::unordered_map<int, std::string> paramTo4ByteIntReg;
        static const std::unordered_map<int, std::string> paramTo1ByteIntReg;
        static const std::unordered_map<int, std::string> paramToFloatReg;
        static const std::unordered_map<std::string, std::string> typeToTempReg;
        static const std::unordered_map<std::string, std::string> typeToTempReg2;
        static const std::unordered_map<std::string, std::string> typeToSizeSuffix;

        /* Keep track of the float constants and which local const num they have,
         * so that already existing float constants do not have to be redefined
         * and declared a second time when encountered. */
        static std::vector<std::pair<float, int>> floatConstantDecs;

        /* Write local constants to another string than the generated code.
         * This way the both of them can be added together and written to file
         * at the end of the assembly generation phase in the correct order. */
        static std::stringstream asmLocalConstants;
        static std::stringstream asmCode;

        static void ResetASMGen();
        static bool GenerateAssemblyFile(std::string filePath);

        static std::string FormatASMStrrrr(std::string asmStr, QuadInstruc quad);
        static std::string FormatASM(std::string op, std::string arg1, std::string arg2, QuadInstruc instr);

        static bool GenerateASMFromQuad();
        static void GenerateASMIDiv(QuadInstruc instr, bool storeRemainder);
        static void GenASMIConditional(std::string setccStr, QuadInstruc instr);
        static void GenASMFConditional(std::string setccStr, QuadInstruc instr);

        /* Calculate the next spawn offset for a variable on the assembly stack. */
        static int NextSpawnOffset(int bytes);

        /* Convert float to its hexadecimal representation to store in assembly output. */
        static std::string DecimalToHexFloatPoint(float f);
        static std::string GenFloatConstDef(float f);
        static void GenFloatASMStore(float f, QuadInstruc instr);
        static bool ContainsFloatConstDec(float f);
        static void GenerateFloatArithmetic(QuadInstruc instr);

        friend class ASMCodeGenVisitor;
        friend class Quad;
        friend int CompileSourceFile(int argc, std::string inputFile, int compilePhase, compFlags flags);
};

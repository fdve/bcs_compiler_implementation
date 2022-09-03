#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <utility>
#include <stack>
#include <unordered_map>
#include <algorithm>

#include "../headers/ast.h"
#include "../headers/symboltable.h"
#include "../headers/asmcodegen.h"
#include "../headers/util.h"
#include "../headers/tac.h"

int ASMCodeGenerator::prevSpawnOffset = 0;
int ASMCodeGenerator::localConstCounter = 0;
bool ASMCodeGenerator::setMainLabelToEntry = 0;

std::string ASMCodeGenerator::x86_64_ENTRY_POINT = "main";
const std::string ASMCodeGenerator::x86_64_FUNC_INTRO = "    pushq    %rbp\n    movq     %rsp,  %rbp\n";
const std::string ASMCodeGenerator::x86_64_FUNC_OUTRO = "     movq    %rbp,  %rsp\n    popq    %rbp\n    retq\n\n";
const std::string ASMCodeGenerator::defaultFilePath = "./asm_out/x86_64_asm_file.s";

const std::unordered_map<std::string, int> ASMCodeGenerator::typeToBytes =
{{"int", 4}, {"float", 4}, {"char", 1}, {"bool", 1}};

const std::unordered_map<int, std::string> ASMCodeGenerator::paramTo4ByteIntReg =
{{1, "%edi"}, {2, "%esi"}, {3, "%edx"}, {4, "%ecx"}, {5, "%r8d"}, {6, "%r9d"}};

const std::unordered_map<int, std::string> ASMCodeGenerator::paramTo1ByteIntReg =
{{1, "%dil"}, {2, "%sil"}, {3, "%dl"}, {4, "%cl"}, {5, "%r8b"}, {6, "%r9b"}};

const std::unordered_map<int, std::string> ASMCodeGenerator::paramToFloatReg =
{{1, "%xmm0"}, {2, "%xmm1"}, {3, "%xmm2"}, {4, "%xmm3"}, {5, "%xmm4"}, {6, "%xmm5"}};

/* Registers where temporary values are being stored, e.g. when moving value from mem to mem. */
const std::unordered_map<std::string, std::string> ASMCodeGenerator::typeToTempReg =
{{"int", "%r11d"}, {"float", "%xmm8"}, {"char", "%r11b"}, {"bool", "%r11b"}};

const std::unordered_map<std::string, std::string> ASMCodeGenerator::typeToTempReg2 =
{{"int", "%r10d"}, {"float", "%xmm9"}, {"char", "%r10b"}, {"bool", "%r10b"}};

std::stringstream ASMCodeGenerator::asmLocalConstants;
std::stringstream ASMCodeGenerator::asmCode;

std::vector<std::pair<float, int>> ASMCodeGenerator::floatConstantDecs;

void ASMCodeGenerator::SetEntryPoint(std::string entryPointStr) {
    ASMCodeGenerator::x86_64_ENTRY_POINT = entryPointStr;
}

/* Generate the x86-64 ASM file.
 * ofstream uses the current working directory as the relative path,
 * not where the file itself using ofstream is residing.
 * The generated assembly file is put in the asm_out directory. */
// bool ASMCodeGenerator::GenerateAssemblyFile(std::vector<TACInstruction> tacInstructions, std::string path) {
bool ASMCodeGenerator::GenerateAssemblyFile(std::string filePath) {
    std::ofstream file_stream;
    // std::string asmFilePath = filePath == "" ? defaultFilePath : filePath;

    file_stream.open(filePath);

    if(!file_stream) {
        std::cerr << "error : could not open file " << filePath << std::endl;
        return false;
    }

    file_stream << asmLocalConstants.str();
    file_stream << ".globl " << x86_64_ENTRY_POINT << "\n\n";
    file_stream << asmCode.str();

    file_stream.close();
    return true;
}

std::string ASMCodeGenerator::FormatASM(std::string op, std::string arg1, std::string arg2, TACInstruction quad) {
    int opSize = op.size();
    std::string emptyChars = "    ";

    if(quad.op == quadOpType::funclabel || quad.op == quadOpType::jmplabel) {
        emptyChars = "";
    }

    int charsBeforeArg1 = 13 - emptyChars.size() - opSize;
    std::string emptyCharsArg1 = charsBeforeArg1 > 0 ? std::string(charsBeforeArg1, ' ') : std::string(5, ' ');

    if(arg2 != "") {
        std::string opAndArg1 = emptyChars + op + emptyCharsArg1 + arg1 + ",";
        int offsetArg2 = 27 - opAndArg1.size() > 0 ? 27 - opAndArg1.size() : 5;


        std::string opAndArgs =  opAndArg1 + std::string(offsetArg2, ' ') + arg2;
        int offsetQuad = 45 - opAndArgs.size() > 0 ? 45 - opAndArgs.size() : 5;

        return opAndArgs + std::string(offsetQuad, ' ') + "# " + TAC::GetQuadStr(quad);
    }
    else {
        std::string result =  emptyChars + op + emptyCharsArg1 + arg1;
        int offset = 45 - result.size() > 0 ? 45 - result.size() : 5;
        return result + std::string(offset, ' ') + "# " + TAC::GetQuadStr(quad);
    }
}

void ASMCodeGenerator::ResetASMGen() {
    prevSpawnOffset = 0;
    localConstCounter = 0;
    floatConstantDecs.clear();
    asmLocalConstants = std::stringstream();
    asmCode = std::stringstream();
}

/* Generate x86-64 ASM from the supplied TAC instructions, put the result into
 * the asmLocalConstants and asmCode stringstreams. */
bool ASMCodeGenerator::GenerateASMFromTAC() {
    /* Reset the static class values used in assembly generation. */
    ResetASMGen();

    /* Store parameter instructions on a stack, so that they can be pushed
     * in x86_64 ASM in reverse order as they were encountered. */
    std::stack<TACInstruction> paramStack;

    std::vector<TACInstruction> funcCallparams;

    std::stack<TACInstruction> funcDecParamStack;

    int numFuncDecParamsInt = 0;
    int numFuncDecParamsFloat = 0;

    for(TACInstruction &instr : TAC::tacInstructions) {
        // std::cout << "asmcodegencpp : GenerateASMFromTAC, current TAC instruction : ";
        // TAC::PrintTACInstruction(instr);

        switch (instr.op) {
            case quadOpType::istore:
                switch (instr.arg1.type) {
                    case quadArgType::intConst: {
                        int value = dynamic_cast<IntConst*>(instr.arg1.nodeRef)->value;
                        int nextSpawnOffset = NextSpawnOffset(4);
                        std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";
                        instr.result.entryRef->ASMAddress = ASMAddress;

                        asmCode << FormatASM("movl", "$" + std::to_string(value), ASMAddress, instr);
                        break;
                    }
                    case quadArgType::boolConst: {
                        int value = dynamic_cast<BoolConst*>(instr.arg1.nodeRef)->value;
                        int nextSpawnOffset = NextSpawnOffset(1);
                        std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";
                        instr.result.entryRef->ASMAddress = ASMAddress;

                        asmCode << FormatASM("movb", "$" + std::to_string(value), ASMAddress, instr);
                        break;
                    }
                    case quadArgType::charConst: {
                        int value = dynamic_cast<CharConst*>(instr.arg1.nodeRef)->value;
                        int nextSpawnOffset = NextSpawnOffset(1);
                        std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";
                        instr.result.entryRef->ASMAddress = ASMAddress;

                        asmCode << FormatASM("movb", "$" + std::to_string(value), ASMAddress, instr);
                        break;
                    }
                    case quadArgType::variable: {
                        if(instr.result.entryRef == nullptr) {
                            cout_str("error : " + instr.result.name + " has no sym table entry ref (nullptr), in asmcodegen().");
                        }

                        /* Allocate a place for the variable if it doesnt have one. */
                        if(instr.result.entryRef->ASMAddress == "") {
                             std::string resultType = instr.arg1.entryRef->type;

                            int nextSpawnOffset = NextSpawnOffset(typeToBytes.at(resultType));
                            std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";
                            instr.result.entryRef->ASMAddress = ASMAddress;
                        }

                        std::unordered_map<std::string, std::string> typeToASMType
                        {{"int", "l"}, {"bool", "b"}, {"char", "b"}};

                        std::string arg1Type = instr.arg1.entryRef->type;
                        std::string ASTType = typeToASMType[arg1Type];
                        std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                        std::string resultAddress = instr.result.entryRef->ASMAddress;

                        asmCode << FormatASM("mov" + ASTType, arg1Address, typeToTempReg.at(arg1Type), instr);
                        asmCode << FormatASM("mov" + ASTType, typeToTempReg.at(arg1Type), resultAddress, instr);
                        break;
                    }
                    default:
                        cout_str("error : unrecognized quadArgType in istore asm gen.");
                        break;
                }
                break;
            case quadOpType::fstore: {
                if(instr.arg1.type == quadArgType::floatConst) {
                    if(instr.arg1.nodeRef == nullptr) {
                        cout_str("error : arg1.nodeRef is a nullptr, in asmcodegen.");
                    }

                    float value = dynamic_cast<FloatConst*>(instr.arg1.nodeRef)->value;

                    if(!ContainsFloatConstDec(value)) {
                        asmLocalConstants << GenFloatConstDef(value);
                    }

                    GenFloatASMStore(value, instr);
                    break;
                }
                else if(instr.arg1.type == quadArgType::variable) {
                    if(instr.result.entryRef == nullptr) {
                        cout_str("ohoh...  " + instr.result.name + " has no sym table entry ref (nullptr)");
                    }

                    if(instr.result.entryRef->ASMAddress == "") {
                        std::string ASMAddress = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
                        instr.result.entryRef->ASMAddress = ASMAddress;
                    }

                    std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                    std::string resultAddress = instr.result.entryRef->ASMAddress;

                    asmCode << FormatASM("movss", arg1Address, typeToTempReg.at("float"), instr);
                    asmCode << FormatASM("movss", typeToTempReg.at("float"),resultAddress, instr);
                    break;
                }
                else {
                    cout_str("error : unrecognized quadArgType in fstore asm gen.");
                    break;
                }
            }
            case quadOpType::iadd: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                asmCode << FormatASM("movl", arg1Address, typeToTempReg.at("int"), instr);

                std::string arg2Address = instr.arg2.entryRef->ASMAddress;
                asmCode << FormatASM("addl", arg2Address, typeToTempReg.at("int"), instr);

                // get a new place on the stack / in register for our variable and move result there
                int nextSpawnOffset = NextSpawnOffset(4);
                std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";

                asmCode << FormatASM("movl", typeToTempReg.at("int"), ASMAddress, instr);

                // store stack position of our just computed  variable
                // SymbolTable::hashmap[instr.result.symTableKey].ASMAddress = ASMAddress;
                instr.result.entryRef->ASMAddress = ASMAddress;
                break;
            }
            case quadOpType::fadd: {
                GenerateFloatArithmetic(instr);
                break;
            }
            case quadOpType::iminus: {
                if(instr.arg1.type == variable) {
                    std::string ASMAddress = instr.arg1.entryRef->ASMAddress;
                    asmCode << FormatASM("movl", ASMAddress, typeToTempReg.at("int"), instr);
                }
                if(instr.arg2.type == variable) {
                    std::string ASMAddress = instr.arg2.entryRef->ASMAddress;
                    asmCode << FormatASM("subl", ASMAddress, typeToTempReg.at("int"), instr);
                }
                // get a new place on the stack / in register for our variable and move result there
                int nextSpawnOffset = NextSpawnOffset(4);
                std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";

                asmCode << FormatASM("movl", typeToTempReg.at("int"), ASMAddress, instr);

                // SymbolTable::hashmap[instr.result.symTableKey].ASMAddress = ASMAddress;
                instr.result.entryRef->ASMAddress = ASMAddress;
                break;
            }
            case quadOpType::fminus: {
                GenerateFloatArithmetic(instr);
                break;
            }
            case quadOpType::idiv: {
                GenerateASMIDiv(instr, false);
                break;
            }
            case quadOpType::fdiv: {
                GenerateFloatArithmetic(instr);
                break;
            }
            case quadOpType::imod: {
                GenerateASMIDiv(instr, true);
                break;
            }
            case quadOpType::imul: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                asmCode << FormatASM("movl", arg1Address, typeToTempReg.at("int"), instr);

                std::string arg2Address = instr.arg2.entryRef->ASMAddress;
                asmCode << FormatASM("imull", arg2Address, typeToTempReg.at("int"), instr);

                // get a new place on the stack / in register for our variable and move result there
                int nextSpawnOffset = NextSpawnOffset(4);
                std::string destAddres = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";

                asmCode << FormatASM("movl", typeToTempReg.at("int"), destAddres, instr);

                instr.result.entryRef->ASMAddress = destAddres;
                break;
            }
            case quadOpType::fmul: {
                GenerateFloatArithmetic(instr);
                break;
            }
            case quadOpType::iunmin: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;

                asmCode << FormatASM("movl", arg1Address, typeToTempReg.at("int"), instr);
                asmCode << FormatASM("negl    ", typeToTempReg.at("int"), "", instr);

                if(instr.result.entryRef->ASMAddress == "") {
                    instr.result.entryRef->ASMAddress = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
                }

                std::string resultAddress = instr.result.entryRef->ASMAddress;
                asmCode << FormatASM("movl", typeToTempReg.at("int"), resultAddress, instr);
                break;
            }
            case quadOpType::funmin: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;

                /* Compute the negation of float x by computing 0 - x. */
                asmCode << FormatASM("pxor", typeToTempReg.at("float"),
                                                  typeToTempReg.at("float"), instr);

                asmCode << FormatASM("subss", arg1Address, typeToTempReg.at("float"), instr);

                if(instr.result.entryRef->ASMAddress == "") {
                    instr.result.entryRef->ASMAddress = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
                }

                std::string resultAddress = instr.result.entryRef->ASMAddress;

                asmCode << FormatASM("movss", typeToTempReg.at("float"), resultAddress, instr);
                break;
            }
            case quadOpType::iunnot: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;

                asmCode << FormatASM("movb", arg1Address, typeToTempReg.at("bool"), instr);
                asmCode << FormatASM("test", typeToTempReg.at("bool"), typeToTempReg.at("bool"), instr);
                asmCode << FormatASM("setz", typeToTempReg.at("bool"), "", instr);

                if(instr.result.entryRef->ASMAddress == "") {
                    instr.result.entryRef->ASMAddress = "-" + std::to_string(NextSpawnOffset(1)) + "(%rsp)";
                }

                std::string resultAddress = instr.result.entryRef->ASMAddress;
                asmCode << FormatASM("movb", typeToTempReg.at("bool"), resultAddress, instr);
                break;
            }
            case quadOpType::ilt: {
                GenerateASMICF("setl", instr);
                break;
            }
            case quadOpType::flt: {
                GenerateASMFCF("setb", instr);
                break;
            }
            case quadOpType::ileq: {
                GenerateASMICF("setle", instr);
                break;
            }
            case quadOpType::fleq: {
                GenerateASMFCF("setbe", instr);
                break;
            }
            case quadOpType::ieq: {
                GenerateASMICF("sete", instr);
                break;
            }
            case quadOpType::feq: {
                GenerateASMFCF("sete", instr);
                break;
            }
            case quadOpType::ineq: {
                GenerateASMICF("setne", instr);
                break;
            }
            case quadOpType::fneq: {
                GenerateASMFCF("setne", instr);
                break;
            }
            case quadOpType::igeq: {
                GenerateASMICF("setge", instr);
                break;
            }
            case quadOpType::fgeq: {
                GenerateASMFCF("setae", instr);
                break;
            }
            case quadOpType::igt: {
                GenerateASMICF("setg", instr);
                break;
            }
            case quadOpType::fgt: {
                GenerateASMFCF("seta", instr);
                break;
            }
            case quadOpType::iand: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                std::string arg2Address = instr.arg2.entryRef->ASMAddress;

                if(instr.result.entryRef->ASMAddress == "") {
                    instr.result.entryRef->ASMAddress = "-" + std::to_string(NextSpawnOffset(1)) + "(%rsp)";
                }

                std::string resultAddress = instr.result.entryRef->ASMAddress;

                asmCode << FormatASM("movb", "$1", typeToTempReg.at("bool"), instr);

                /* Clear the register where the args are checked for their bool value. */
                asmCode << FormatASM("xorb ", typeToTempReg2.at("bool"),
                                                 typeToTempReg2.at("bool"), instr);

                /* If either the first or second argument of a && b is false (0), move 0 to the result. */
                asmCode << FormatASM("movb", arg1Address, typeToTempReg2.at("bool"), instr);
                asmCode << FormatASM("test", typeToTempReg2.at("bool"), typeToTempReg2.at("bool"), instr);
                asmCode << FormatASM("cmovz", typeToTempReg2.at("int"), typeToTempReg.at("int"), instr);

                asmCode << FormatASM("movb", arg2Address, typeToTempReg2.at("bool"), instr);
                asmCode << FormatASM("test", typeToTempReg2.at("bool"), typeToTempReg2.at("bool"), instr);
                asmCode << FormatASM("cmovz", typeToTempReg2.at("int"), typeToTempReg.at("int"), instr);

                asmCode << FormatASM("movb", typeToTempReg.at("bool"), resultAddress, instr);
                break;
            }
            case quadOpType::ior: {
                std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                std::string arg2Address = instr.arg2.entryRef->ASMAddress;

                /* Cant compare mem to mem, so move first argument to register. */
                asmCode << FormatASM("movb", arg1Address, typeToTempReg.at("bool"), instr);

                /* Logical or the first argument, now stored in ecx, with the second bool arg.*/
                asmCode << FormatASM("or", arg2Address, typeToTempReg.at("bool"), instr);

                if(instr.result.entryRef->ASMAddress == "") {
                    instr.result.entryRef->ASMAddress = "-" + std::to_string(NextSpawnOffset(1)) + "(%rsp)";
                }

                asmCode << FormatASM("setnz", instr.result.entryRef->ASMAddress, "", instr);
                break;
            }
            case quadOpType::jmp: {
                /* If arg1 name is undefined, the quad is of the form goto label, i.e.
                 * jump directly to the label. */
                if(instr.arg1.name == "") {
                    asmCode << FormatASM("jmp", instr.result.label, "", instr);
                    break;
                }
                else {
                    std::string arg1Address = instr.arg1.entryRef->ASMAddress;
                    std::string jmpLabel = instr.result.label;

                    /* If the first conditional argument of the quad is false, jump to the given lable. */
                    asmCode << FormatASM("cmpb", "$0", arg1Address, instr);
                    asmCode << FormatASM("je", jmpLabel, "", instr);
                    break;
                }
            }
            case quadOpType::jmplabel: {
                asmCode << FormatASM(instr.result.label + ":", "", "", instr);
                break;
            }
            case quadOpType::param: {
                /* If encountering a param in a function call, push it to the param stack.
                 * If encountering a param in a function declaration, give it the correct address. */

                if(instr.arg1.label != "decl_param") {
                    paramStack.push(instr);
                    funcCallparams.push_back(instr);
                }
                else {
                    // funcDecParamStack.push(instr);
                    // int paramNum = std::stoi(instr.arg2.label);

                    std::string paramReg;

                    if(instr.arg1.entryRef->type == "float") {
                        paramReg = paramToFloatReg.at(++numFuncDecParamsFloat);
                    }
                    else if(instr.arg1.entryRef->type == "int") {
                        paramReg = paramTo4ByteIntReg.at(++numFuncDecParamsInt);
                    }
                    else {
                        paramReg = paramTo1ByteIntReg.at(++numFuncDecParamsInt);
                    }

                    /* If encountered function decl param is last param,
                     * reset the function decl param types counters. */
                    if(instr.arg2.label == "lastParam") {
                        numFuncDecParamsInt = 0;
                        numFuncDecParamsFloat = 0;
                    }

                    // instr.arg1.entryRef->ASMAddress = callRegisters32Bit.at(paramNum);
                    instr.arg1.entryRef->ASMAddress = paramReg;
                }

                break;
            }
            case quadOpType::call: {
                /* (1) Check how many params there are. */
                std::cout << "quadOpType asmcodegen, call, num of params = " + std::stoi(instr.arg2.label) << std::endl;

                /* Depending on the type of the param, move param to correct func call arg register.  */


                // std::unordered_map<std::string, int> typeToInstrSuffix
                // {{"int", "l"}, {"float", "ss"}, {"char", "b"}, {"bool", "b"}};

                int paramNumsInt = 0;
                int paramNumsFloat = 0;

                /* (2) Passing params in reverse order to subroutine, in order
                 * rdi, rsi, rdx, rcx, r8, r9. */

                 std::reverse(funcCallparams.begin(), funcCallparams.end());
                 std::vector<std::string> moveFncArgs;

                 while(funcCallparams.size() != 0) {
                     int paramNumber = funcCallparams.size();

                     // need location of source
                     TACInstruction paramInstr = funcCallparams.back();
                     funcCallparams.pop_back();

                     std::string targetFncCallReg;
                     std::string movInstr = "mov";

                     if(paramInstr.arg1.entryRef->type == "float") {
                         targetFncCallReg = paramToFloatReg.at(++paramNumsFloat);
                         movInstr += "ss";
                     }
                     else if(paramInstr.arg1.entryRef->type == "int") {
                         targetFncCallReg = paramTo4ByteIntReg.at(++paramNumsInt);
                         movInstr += "l";
                     }
                     else {
                         targetFncCallReg = paramTo1ByteIntReg.at(++paramNumsInt);
                         movInstr += "b";
                     }

                     std::string paramASMAddress = paramInstr.arg1.entryRef->ASMAddress;

                     // asmCode << FormatASM(movInstr, paramASMAddress, targetFncCallReg, paramInstr);
                     moveFncArgs.push_back(FormatASM(movInstr, paramASMAddress, targetFncCallReg, paramInstr));


                     // need mov with correct size op, and need to move to correct reg

                     // std::string ASMTargetAddress = paramNumToIntReg[paramNumber];
                     // asmCode << FormatASM("movl    " + paramASMAddress + ",  " + ASMTargetAddress, paramInstr);
                 }

                 /* Move arguments in reversed order. */
                 std::reverse(moveFncArgs.begin(), moveFncArgs.end());
                 for(std::string moveFncArg : moveFncArgs) {
                     asmCode << moveFncArg;
                 }


                // while(!paramStack.empty()) {
                //     int paramNumber = paramStack.size();
                //
                //     // need location of source
                //     TACInstruction paramInstr = paramStack.top();
                //     paramStack.pop();
                //
                //     std::string targetFncCallReg;
                //     std::string movInstr = "mov";
                //
                //     if(paramInstr.arg1.entryRef->type == "float") {
                //         targetFncCallReg = paramToFloatReg.at(++paramNumsFloat);
                //         movInstr += "ss";
                //     }
                //     else if(paramInstr.arg1.entryRef->type == "int") {
                //         targetFncCallReg = paramTo4ByteIntReg.at(++paramNumsInt);
                //         movInstr += "l";
                //     }
                //     else {
                //         targetFncCallReg = paramTo1ByteIntReg.at(++paramNumsInt);
                //         movInstr += "b";
                //     }
                //
                //     std::string paramASMAddress = paramInstr.arg1.entryRef->ASMAddress;
                //
                //     asmCode << FormatASM(movInstr, paramASMAddress, targetFncCallReg, paramInstr);
                //
                //     // need mov with correct size op, and need to move to correct reg
                //
                //     // std::string ASMTargetAddress = paramNumToIntReg[paramNumber];
                //     // asmCode << FormatASM("movl    " + paramASMAddress + ",  " + ASMTargetAddress, paramInstr);
                // }
                asmCode << FormatASM("call", instr.arg1.label, "", instr);

                /* If call return type is non void, store the returned result. */
                /* - [] add 1 byte / float support etc. */
                // if(instr.result.name != "") {

                /* There will be an entry for the result argument of the call quad
                 * if the function call is of type non void. */
                if(instr.result.entryRef != nullptr) {
                    std::string resultArgType = instr.result.entryRef->type;

                    int nextSpawnOffset = NextSpawnOffset(typeToBytes.at(resultArgType));
                    std::string ASMAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";
                    instr.result.entryRef->ASMAddress = ASMAddress;

                    if(resultArgType == "int") {
                        asmCode << FormatASM("movl", "%eax", ASMAddress, instr);
                    }
                    else if(resultArgType == "float") {
                        asmCode << FormatASM("movss", "%xmm0", ASMAddress, instr);
                    }
                    else {
                        asmCode << FormatASM("movb", "%al", ASMAddress, instr);
                    }
                }
                break;
            }
            case quadOpType::funclabel:
                /* Add the function prologue at this point as well?  */

                /* If the encountered func label is main, set it to the entry point.
                 * Default global symbol for the entry point is main. */
                if(instr.result.label == "main" && setMainLabelToEntry) {
                    instr.result.label = x86_64_ENTRY_POINT;
                }

                asmCode << "\n" << FormatASM(instr.result.label + ":", "", "", instr);
                asmCode << ASMCodeGenerator::x86_64_FUNC_INTRO;
                break;
            case quadOpType::ireturn: {
                /* Check if a value is being returned. Return calls which do not return
                 * anything are of type ireturn and have their first quad arg label set to void. */

                if(instr.arg1.label != "void") {
                    std::string movOp = instr.arg1.entryRef->type == "int" ? "movl" : "movb";
                    std::string resultReg = instr.arg1.entryRef->type == "int" ? "%eax" : "%al";
                    asmCode << FormatASM(movOp, instr.arg1.entryRef->ASMAddress, resultReg, instr);
                }

                asmCode << FormatASM("leave", "", "", instr);
                asmCode << FormatASM("ret", "", "", instr);
                break;
            }
            case quadOpType::freturn: {
                asmCode << FormatASM("movss", instr.arg1.entryRef->ASMAddress, "%xmm0", instr);
                asmCode << FormatASM("leave", "", "", instr);
                asmCode << FormatASM("ret", "", "", instr);
                break;
            }
            case quadOpType::nop: {
                asmCode << FormatASM("nop", "", "", instr);
                break;
            }
            default:
                cout_str("error : unrecognized quadOpType : " + std::to_string(instr.op) + " in GenerateASMFromTAC()");
                return false;
                break;
        }
    }
    return true;
}

/* Generate signed integer division asm and store the quotient or the remainder in the result field. */
void ASMCodeGenerator::GenerateASMIDiv(TACInstruction instr, bool storeRemainder) {
    /* Move dividend into %eax. */
    std::string AddressDividend = instr.arg1.entryRef->ASMAddress;
    asmCode << FormatASM("movl", AddressDividend, "%eax", instr);

    /* Save current value of %ecx on the stack since there may be a fnc call arg stored. */
    std::string temporaryAddressEcx = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
    asmCode << FormatASM("movl", "%ecx", temporaryAddressEcx, instr);

    /* Save current value of %edx on the stack since there may be a fnc call arg stored. */
    std::string temporaryAddressEdx = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
    asmCode << FormatASM("movl", "%edx", temporaryAddressEdx, instr);

    /* Move divisor into %ecx. */
    std::string AddressDivisor = instr.arg2.entryRef->ASMAddress;
    asmCode << FormatASM("movl", AddressDivisor, "%ecx", instr);

    asmCode << FormatASM("cltd", "", "", instr);
    asmCode << FormatASM("idivl", "%ecx", "", instr);

    /* If the result quad argument has no asm address, i.e. when dealing
     * with a new compiler temporary, give the temporary an address.  */
    if(instr.result.entryRef->ASMAddress == "") {
        instr.result.entryRef->ASMAddress = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
    }

    std::string divResultReg = storeRemainder ? "%edx" : "%eax";

    std::string AddressResultVar = instr.result.entryRef->ASMAddress;
    asmCode << FormatASM("movl", divResultReg, AddressResultVar, instr);

    /* Restore result of %ecx and %edx. */
    asmCode << FormatASM("movl", temporaryAddressEcx , "%ecx", instr);
    asmCode << FormatASM("movl", temporaryAddressEdx , "%edx", instr);

}

/* -[] change name? its not necessarily for control flow, e.g. bool b = a || c;  */
/* -[] add size suffix?  */
/* Generate conditional ASM for integer operands control flow. */
void ASMCodeGenerator::GenerateASMICF(std::string setccStr, TACInstruction instr) {
    /* -[]  byte vs int? */

    /* Depending on the types of the arguments, use appropriate size temp register
    * to store the conditional compare result.  */
    // std::string tempRegister = temporaryRegs[instr.arg1.entryRef->type];
    std::string arg1Type = instr.arg1.entryRef->type;

    std::string arg1Address = instr.arg1.entryRef->ASMAddress;
    std::string arg2Address = instr.arg2.entryRef->ASMAddress;

    /* Compare both arguments of the conditional. Both args reside on the stack,
    * so first move one into a register, since its not possible to compare mem to mem. */
    asmCode << FormatASM("mov", arg1Address, typeToTempReg.at(arg1Type), instr);

    /* In AT&T Syntax, when comparing a and b, e.g. jump if less (jl) will be true when b < a
     * so swith the order of the operands. */
    asmCode << FormatASM("cmp", arg2Address, typeToTempReg.at(arg1Type), instr);

    /* Store the result of the comparison in a new byte location on the stack. */
    std::string boolResultLoc = "-" + std::to_string(NextSpawnOffset(1)) + "(%rsp)";
    asmCode << FormatASM(setccStr, boolResultLoc, "", instr);

    /* Set the address of the result quad arg to the boolean result location. */
    instr.result.entryRef->ASMAddress = boolResultLoc;
}

/* *-[] change name? its not necessarily for control flow, e.g. bool b = a || c;  /
/* Generate conditional ASM for float operands control flow. */
void ASMCodeGenerator::GenerateASMFCF(std::string setccStr, TACInstruction instr) {
    std::string arg1Address = instr.arg1.entryRef->ASMAddress;
    std::string arg2Address = instr.arg2.entryRef->ASMAddress;
    std::string boolResultLoc = "-" + std::to_string(NextSpawnOffset(1)) + "(%rsp)";

    /* Compare both arguments of the conditional. Both args reside on the stack,
    * so first move one into a register, since its not possible to compare mem to mem. */
    asmCode << FormatASM("movss", arg1Address, typeToTempReg.at("float"), instr);

    /* In AT&T Syntax, the 2nd arg in the cmp instruc is the destination, while the first one
     * is the source. Meaning that when cmp a, b, jump if less (jl) will be true when b < a. */
    asmCode << FormatASM("comiss", arg2Address, typeToTempReg.at("float"), instr);

    asmCode << FormatASM(setccStr, boolResultLoc, "", instr);

    instr.result.entryRef->ASMAddress = boolResultLoc;
}

void ASMCodeGenerator::GenerateFloatArithmetic(TACInstruction instr) {
    std::unordered_map<int, std::string> opToFloatASM
    {{quadOpType::fadd, "addss"}, {quadOpType::fminus, "subss"},
     {quadOpType::fmul, "mulss"}, {quadOpType::fdiv, "divss"}};

    /* Get location of the 1st variable.  */
    std::string arg1Address = instr.arg1.entryRef->ASMAddress;
    asmCode << FormatASM("movss", arg1Address, typeToTempReg.at("float"), instr);

    /* Get location of the 2nd variable.  */
    std::string arg2Address = instr.arg2.entryRef->ASMAddress;

    asmCode << FormatASM(opToFloatASM[instr.op], arg2Address, typeToTempReg.at("float"), instr);


    // get a new place on the stack / in register for our variable and move result there
    int nextSpawnOffset = NextSpawnOffset(4);
    std::string resultAddress = "-" + std::to_string(nextSpawnOffset) + "(%rsp)";

    asmCode << FormatASM("movss", typeToTempReg.at("float"), resultAddress, instr);

    // store stack position of our just computed  variable
    // SymbolTable::hashmap[instr.result.symTableKey].ASMAddress = ASMAddress;
    instr.result.entryRef->ASMAddress = resultAddress;
}

/* Calculate the next spawn offset for a certain variable_size (in bytes) on
 * the assembly stack. 4 byte vars (int and float) need to be 4 byte alligned.
 * 1 byte vars (char and bool) need to be 1 byte alligned, and can be placed
 * 1 byte off other variables on the stack. */
int ASMCodeGenerator::NextSpawnOffset(int varSize) {
    size_t newSpawnOffset = 0;

    if(varSize == 4) {
        if(prevSpawnOffset == 0) {
            newSpawnOffset = 4;
        }
        else if(prevSpawnOffset % 4 == 1) {
            newSpawnOffset = prevSpawnOffset + 7;
        }
        else if(prevSpawnOffset % 4 == 2) {
            newSpawnOffset = prevSpawnOffset + 6;
        }
        else if(prevSpawnOffset % 4 == 3) {
            newSpawnOffset = prevSpawnOffset + 5;
        }
        else if(prevSpawnOffset % 4 == 0) {
            newSpawnOffset = prevSpawnOffset + 4;
        }
    }
    else if(varSize == 1) {
        newSpawnOffset = prevSpawnOffset + 1;
    }
    else {
        cout_str("error : unrecognized var size in nextSpawnOffset()");
    }

    prevSpawnOffset = newSpawnOffset;
    return newSpawnOffset;
}

/* Convert a decimal float to its hexadecimal representation. */
std::string ASMCodeGenerator::DecimalToHexFloatPoint(float f) {
    int i = *(reinterpret_cast<int*>(&f));
    std::stringstream ss;
    ss << "0x" << std::hex << i;
    return ss.str();
}

/* Generate an x86-64 float ASM constant string. */
std::string ASMCodeGenerator::GenFloatConstDef(float f) {
    std::string hexaFloatString = DecimalToHexFloatPoint(f);

    std::stringstream floatConstDefSStream;
    floatConstDefSStream << ".LC" << localConstCounter
    << ":\n" << "        .long    " << hexaFloatString << "\n\n";

    return floatConstDefSStream.str();
}

/* Generate x86-64 float instruction string. */
void ASMCodeGenerator::GenFloatASMStore(float f, TACInstruction instr) {
    int constantNum = localConstCounter;

    if(ContainsFloatConstDec(f)) {
        for(const std::pair<float, int> &floatPair : floatConstantDecs) {
            if(f == floatPair.first) {
                constantNum = floatPair.second;
            }
        }
    }
    else {
        std::pair<float, int> floatConstantDec (f, localConstCounter);
        floatConstantDecs.push_back(floatConstantDec);
        localConstCounter++;
    }

    std::string ASMAddress = "-" + std::to_string(NextSpawnOffset(4)) + "(%rsp)";
    instr.result.entryRef->ASMAddress = ASMAddress;

    asmCode << FormatASM("movss", ".LC" + std::to_string(constantNum) + "(%rip)",
                                      typeToTempReg.at("float"), instr);

    asmCode << FormatASM("movss", typeToTempReg.at("float"), ASMAddress, instr);
}

/* Check whether the given float has already been declared as a constant. */
bool ASMCodeGenerator::ContainsFloatConstDec(float f) {
    for(const std::pair<float, int> &floatPair : floatConstantDecs) {
        if(f == floatPair.first) {
            return true;
        }
    }
    return false;
}

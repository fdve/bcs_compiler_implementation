/* Google test file, where tests are run on the different compiler phases.
 * For now and due to time constrains, only the back-end and the front-end
 * have been tested.
 *
 * The front-end tests test various aspect of the front-end of the compiler,
 * such as checking correct types, correct functions calls, program with incorrect
 * grammar. The path of the test file is given to the compiler, which will compile
 * the files only with the front-end of the compiler. During this compilation process,
 * the error logger will store error messages as erors are encountered, such as
 * an assignment a = b in which type of a is incompatible with the type of b.
 * Finally, after the front-end is done and the error logger has stored all its error messages,
 * the front-end gtest will compare which error messages it expects for the given test file
 * to the actual generated error messages by the error logger.
 *
 * The back-end tests work by first compiling the compiler. Afterwards, the compiler
 * is used to generate the assembly for the given input test files passing them through
 * the compilers front-, middle- and back-end. Now that x86_64 ASM has been generated for
 * the given input star source file, that x86_64 ASM is being compiled together
 * with the gtests below, so that, the Gtest can find the generated x86_64 ASM function
 * definitions. Finally, the x86_64 ASM functions are run, and return a certain value. This value
 * is being compared to what the gtest expects, and report an error if the generated
 * x86_64 ASM file returned value is different than what the gtest expected. */

#include <gtest/gtest.h>
#include <iostream>

#include "../headers/main.h"
#include "../headers/util.h"
#include "../headers/errorlogger.h"
#include "../headers/symboltable.h"

extern int yyparse();
extern FILE *yyin;
extern void InitFlex();

/* Test parsing all parse rules according to the syntax. */
TEST(FrontEndTests, ParserTest1) {
	ErrLog::Reset();
	InitFlex();
 	std::string filePath = "./source/test_framework/test_files/front-end/parser1.star";
 	yyin = fopen(filePath.c_str(), "r");
    yyparse();
 	EXPECT_EQ(ErrLog::parsingError, false);
}

/* Should not parse correctly, start() given with parameters.  */
TEST(FrontEndTests, ParserTest2) {
	ErrLog::Reset();
	InitFlex();
 	std::string filePath = "./source/test_framework/test_files/front-end/parser2.star";
 	yyin = fopen(filePath.c_str(), "r");
    yyparse();
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::ParserError(1, 11, "syntax error, unexpected DATATYPE, expecting RPARENT"));
}

/* Out of scope variables and redefining variables. */
/* -[] this one is incomplete atm*/
TEST(FrontEndTests, ScopeTest1) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope1.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 4);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::UndefinedVar(ErrLog::CreateInfo(2,5), "a"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) == ErrLog::RedefineVar(ErrLog::CreateInfo(9,17), "d"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(2) == ErrLog::UndefinedVar(ErrLog::CreateInfo(12,9), "b"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(3) == ErrLog::RedefineVar(ErrLog::CreateInfo(18,11), "start"));
}

/* Correct usage of scopes. */
TEST(FrontEndTests, ScopeTest2) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope2.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 0);
}

/* Redefining function ID's as variables. */
TEST(FrontEndTests, ScopeTest3) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope3.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 2);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::RedefineVar(ErrLog::CreateInfo(8,9), "f"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) == ErrLog::RedefineVar(ErrLog::CreateInfo(9, 11), "start"));
}

/* Testing function parameters. */
TEST(FrontEndTests, ScopeTest4) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope4.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 0);
}

/* Testing types of returned values, e.g. void fnc does not return a value. */
TEST(FrontEndTests, TypeTestReturnValues) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/type_ret.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) ==  ErrLog::ReturnError(ErrLog::CreateInfo(3,12), "start", "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) ==  ErrLog::ReturnError(ErrLog::CreateInfo(8,12), "fnc1", "void", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(2) == ErrLog::ReturnError(ErrLog::CreateInfo(13,10), "fnc2", "float", "void"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(3) == ErrLog::ReturnError(ErrLog::CreateInfo(18,12), "fnc3", "void", "bool"));
}

/* Testing incompatible types, e.g. bool b = 123.213. */
TEST(FrontEndTests, TypeTestIncompTypes) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/type_compatibility.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 14);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(5,9), "variableDef", "A", "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(9,5), "assignment", "B", "int", "bool"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(2) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(12,10), "variableDef", "c", "char", "void"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(3) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(15,9), "variableDef", "d", "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(4) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(18,5),"unaryOp", "unknownUnOpName", "bool", "char"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(5) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(21,9), "unaryOp", "unknownUnOpName", "bool", "int"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(6) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(24,8), "ifStmnt", "unknownIfStmntName", "char", "bool"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(7) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(27,13), "unaryOp", "unknownUnOpName", "bool", "int"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(8) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(35,5), "assignment", "z", "float", "int"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(9) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(38,5), "assignment", "tempf", "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(10) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(41,9), "variableDef", "zzz", "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(11) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(44,20), "for", "unknownForStmnttName", "float", "bool"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(12) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(47,11), "while", "unknownDoWhileStmnttName", "int", "bool"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(13) == ErrLog::IncompatibleTypes(ErrLog::CreateInfo(50,17), "doWhile", "unknownWhileStmnttName", "float", "bool"));
}

/* Testing incompatible binary op types, e.g. bool b = true * 0.3. */
TEST(FrontEndTests, TypeTestIncompBinOpTypes) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/type_compatibility_binop.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 4);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::IncompatibleTypesBinOps(ErrLog::CreateInfo(3,20), "*", "bool", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) == ErrLog::IncompatibleTypesBinOps(ErrLog::CreateInfo(6,17), "*", "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(2) == ErrLog::IncompatibleTypesBinOps(ErrLog::CreateInfo(9,12), "-", "bool", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(3) == ErrLog::IncompatibleTypesBinOps(ErrLog::CreateInfo(15,20), "/", "char", "float"));
}

/* Testing invalid types of params, e.g. fnc(char) being called with fnc(true).*/
TEST(FrontEndTests, TypeTestInvalidTypeParams) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/type_invalid_params.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 2);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::InvalidTypesParams(ErrLog::CreateInfo(7,5), "fnc3", 1, "int", "float"));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) == ErrLog::InvalidTypesParams(ErrLog::CreateInfo(7,5), "fnc3", 3, "bool" , "char"));
}

/* Test that declared functions contain a maximum of 6 arguments. */
TEST(FrontEndTests, TooManyFuncParams) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/too_many_args.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 1);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) ==  ErrLog::FncDecTooManyParams(ErrLog::CreateInfo(1,1), "many_args"));
}

/* Testing that a function is called with the correct num of params. */
TEST(FrontEndTests, InvalidNumParams) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/fnccall_wrong_num_params.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 3);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::InvalidNumParams(ErrLog::CreateInfo(5,5), "fnc3", 3, 4));
	EXPECT_TRUE(ErrLog::errorMsgs.at(1) == ErrLog::InvalidNumParams(ErrLog::CreateInfo(8,5), "fnc3", 3, 0));
	EXPECT_TRUE(ErrLog::errorMsgs.at(2) == ErrLog::InvalidNumParams(ErrLog::CreateInfo(11,5), "start", 0, 5));
}

/* Testing that functions which are tried to be called are actually functions. */
TEST(FrontEndTests, CallingFakeFnc) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/calling_fake_fnc.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 1);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) == ErrLog::CalledSymbolIsNotFnc(ErrLog::CreateInfo(5,5), "fake_function"));
}

/* Testing invalid declared char. */
TEST(FrontEndTests, InvalidCharDecl) {
	ErrLog::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/invalid_char.star";
	CompileSourceFile(2, filePath, CompilePhase::front);
	EXPECT_EQ(ErrLog::errorMsgs.size(), 1);
	EXPECT_TRUE(ErrLog::errorMsgs.at(0) ==  ErrLog::IllegitimateChar(ErrLog::CreateInfo(2,14), "many_args"));
}

/* Testing arithmetic and logical operators precendence. */
extern "C" int test_asm_op_prec();
TEST(BackEndTests, AsmOpPrec) {
	EXPECT_EQ(test_asm_op_prec(), 0);
}

/* Testing integer asm generated arithmetic. */
extern "C" int test_asm_arithmetic();
TEST(BackEndTests, AsmArithmetic) {
	EXPECT_EQ(test_asm_arithmetic(), 79);
}

/* Testing integer asm generated negation. */
extern "C" int test_asm_neg();
TEST(BackEndTests, AsmNegation) {
	EXPECT_EQ(test_asm_neg(), -100);
}

/* Testing integer simple asm assignment. */
extern "C" int test_asm_assign();
TEST(BackEndTests, AsmAssign) {
	EXPECT_EQ(test_asm_assign(), 215);
}

/* Testing integer asm generated control flow with bool expressions. */
extern "C" int test_asm_cf();
TEST(BackEndTests, AsmControlFlow) {
	EXPECT_EQ(test_asm_cf(), 111);
}

/* Testing bool simple function call. */
extern "C" int test_asm_call_bool();
TEST(BackEndTests, AsmCallBool) {
	EXPECT_EQ(test_asm_call_bool(), 123);
}

/* Testing integer simple function call. */
extern "C" int test_asm_call_simple1();
TEST(BackEndTests, AsmCallSimple1) {
	EXPECT_EQ(test_asm_call_simple1(), 1300);
}

/* Testing integer simple function call. */
extern "C" int test_asm_call_simple2();
TEST(BackEndTests, AsmCallSimple2) {
	EXPECT_EQ(test_asm_call_simple2(), 28);
}

/* Testing function call with mixed datatype of parameters.. */
extern "C" int test_asm_call_mixed();
TEST(BackEndTests, AsmCallMixed) {
	EXPECT_EQ(test_asm_call_mixed(), 2);
}

/* Testing float asm arithmetic.  */
extern "C" float test_asm_float_arithmetic();
TEST(BackEndTests, AsmFloatSimple) {
	EXPECT_FLOAT_EQ(test_asm_float_arithmetic(), -2.3);
}

/* Testing float conditionals in control flow. */
extern "C" float test_asm_float_cf();
TEST(BackEndTests, AsmFloatControlFlow) {
	EXPECT_FLOAT_EQ(test_asm_float_cf(), 13.37);
}

/* Testing float negation. */
extern "C" float test_asm_float_neg();
TEST(BackEndTests, AsmFloatNegation) {
	EXPECT_FLOAT_EQ(test_asm_float_neg(), -123.456);
}

/* Testing float negation. */
extern "C" float test_asm_float_assign();
TEST(BackEndTests, AsmFloatAssign) {
	EXPECT_FLOAT_EQ(test_asm_float_assign(), 12.34);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

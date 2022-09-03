/* Google test file, where tests are run on the different compiler phases. */

#include <gtest/gtest.h>
#include <iostream>

#include "../headers/main.h"
#include "../headers/util.h"
#include "../headers/errorlogger.h"
#include "../headers/symboltable.h"

extern int yyparse();
extern FILE *yyin;
extern void InitFlex();

/* Test parsing all parse rules, with Star code according to the syntax. */
TEST(FrontEndTests, ParserTest1) {
	ErrorLogger::Reset();
	InitFlex();
 	std::string filePath = "./source/test_framework/test_files/front-end/parser1.star";
 	yyin = fopen(filePath.c_str(), "r");
    yyparse();
 	EXPECT_EQ(ErrorLogger::parsingError, false);
}

/* Should not parse correctly, start() given with parameters.  */
TEST(FrontEndTests, ParserTest2) {
	ErrorLogger::Reset();
	InitFlex();
 	std::string filePath = "./source/test_framework/test_files/front-end/parser2.star";
 	yyin = fopen(filePath.c_str(), "r");
    yyparse();
 	EXPECT_EQ(ErrorLogger::parsingError, true);
}

/* Out of scope variables and redefining variables. */
/* -[] this one is incomplete atm*/
TEST(FrontEndTests, ScopeTest1) {
	ErrorLogger::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope1.star";
	CompileSourceFile(1, filePath, CompilePhase::front);
	EXPECT_EQ(ErrorLogger::errorMsgs.size(), 4);
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(0) == ErrorLogger::UndefinedVar(ErrorLogger::CreateInfo(2,5), "a"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(1) == ErrorLogger::RedefineVar(ErrorLogger::CreateInfo(9,17), "d"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(2) == ErrorLogger::UndefinedVar(ErrorLogger::CreateInfo(12,9), "b"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(3) == ErrorLogger::RedefineVar(ErrorLogger::CreateInfo(18,11), "start"));
}

/* Correct usage of scopes. */
TEST(FrontEndTests, ScopeTest2) {
	ErrorLogger::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope2.star";
	CompileSourceFile(1, filePath, CompilePhase::front);
	EXPECT_EQ(ErrorLogger::errorMsgs.size(), 0);
}

/* Redefining function ID's as variables. */
TEST(FrontEndTests, ScopeTest3) {
	ErrorLogger::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope3.star";
	CompileSourceFile(1, filePath, CompilePhase::front);
	EXPECT_EQ(ErrorLogger::errorMsgs.size(), 2);
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(0) == ErrorLogger::RedefineVar(ErrorLogger::CreateInfo(8,9), "f"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(1) == ErrorLogger::RedefineVar(ErrorLogger::CreateInfo(9, 11), "start"));
}

/* Testing function parameters. */
TEST(FrontEndTests, ScopeTest4) {
	ErrorLogger::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/scope4.star";
	CompileSourceFile(1, filePath, CompilePhase::front);
	EXPECT_EQ(ErrorLogger::errorMsgs.size(), 0);
}

/* - [] clean up this fucntion? */
/* Testing type anaylysis (incompatible operator types, return values etc). */
TEST(FrontEndTests, TypeTest1) {
	ErrorLogger::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/type1.star";
	CompileSourceFile(1, filePath, CompilePhase::front);
	EXPECT_EQ(ErrorLogger::errorMsgs.size(), 28);
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(0) ==  ErrorLogger::ReturnError(ErrorLogger::CreateInfo(5,12), "start", "int", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(1) ==  ErrorLogger::ReturnError(ErrorLogger::CreateInfo(10,12), "fnc1", "void", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(2) == ErrorLogger::ReturnError(ErrorLogger::CreateInfo(15,10), "fnc2", "float", "void"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(3) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(24,9), "variableDef", "A", "int", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(4) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(28,5), "assignment", "B", "int", "bool"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(5) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(34,10), "variableDef", "c", "char", "void"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(6) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(37,9), "variableDef", "d", "int", "bool"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(7) == ErrorLogger::IncompatibleTypesBinOps(ErrorLogger::CreateInfo(37,19), "*", "bool", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(8) == ErrorLogger::IncompatibleTypesBinOps(ErrorLogger::CreateInfo(40,17), "*", "int", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(9) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(43,5),"unaryOp", "unknownUnOpName", "bool", "char"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(10) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(46,9), "unaryOp", "unknownUnOpName", "bool", "int"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(11) == ErrorLogger::IncompatibleTypesBinOps(ErrorLogger::CreateInfo(52,12), "*", "bool", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(12) == ErrorLogger::CalledSymbolIsNotFnc(ErrorLogger::CreateInfo(59,5), "fake_function"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(13) == ErrorLogger::InvalidTypesParams(ErrorLogger::CreateInfo(62,5), "fnc3", 3, "bool", "char"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(14) == ErrorLogger::InvalidTypesParams(ErrorLogger::CreateInfo(62,5), "fnc3", 1, "int" , "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(15) == ErrorLogger::InvalidNumParams(ErrorLogger::CreateInfo(68,5), "fnc3", 3, 4));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(16) == ErrorLogger::InvalidNumParams(ErrorLogger::CreateInfo(71,5), "fnc3", 3, 0));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(17) == ErrorLogger::InvalidNumParams(ErrorLogger::CreateInfo(74,5), "start", 0, 5));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(18) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(80,8), "ifStmnt", "unknownIfStmntName", "char", "bool"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(19) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(83,13), "unaryOp", "unknownUnOpName", "bool", "int"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(20) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(89,5), "assignment", "z", "float", "int"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(21) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(96,5), "assignment", "tempf", "int", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(22) == ErrorLogger::IncompatibleTypesBinOps(ErrorLogger::CreateInfo(96,16), "+", "float", "int"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(23) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(99,9), "variableDef", "zzz", "int", "float"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(24) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(105,20), "for", "unknownForStmnttName", "float", "bool"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(25) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(111,11), "while", "unknownDoWhileStmnttName", "int", "bool"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(26) == ErrorLogger::IncompatibleTypes(ErrorLogger::CreateInfo(114,17), "doWhile", "unknownWhileStmnttName", "float", "bool"));
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(27) == ErrorLogger::ReturnError(ErrorLogger::CreateInfo(117,12), "fnc4", "void", "bool"));
}

/* Test that declared functions contain a maximum of 6 arguments. */
TEST(FrontEndTests, TooManyFuncParams) {
	ErrorLogger::Reset();
	InitFlex();
	std::string filePath = "./source/test_framework/test_files/front-end/too_many_args.star";
	CompileSourceFile(1, filePath, CompilePhase::front);
	EXPECT_EQ(ErrorLogger::errorMsgs.size(), 1);
	EXPECT_TRUE(ErrorLogger::errorMsgs.at(0) ==  ErrorLogger::FncDecTooManyParams(ErrorLogger::CreateInfo(1,1), "many_args"));
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

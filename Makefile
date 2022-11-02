# Makefile where the source files from the source directory are compiled.
#
# Run "make gtest; ./run_gtest" to compile and run the test framework.
#
# Use "make compiler; ./star_compiler ./path-to-your-file" to compile the star compiler
# and use the star compiler to compile your given Star source file into x86_64 ASM.
# The generated assembly file is placed in the root asm_out folder.

MAINDIR = ./source/main
FRONTDIR = ./source/front-end
MIDDIR = ./source/middle-end
BACKDIR = ./source/back-end
GTESTASMDIR = ./source/test_framework/test_files/gen_asm_files
GLOBALDIR = ./source/global
HEADERSDIR = ./source/headers
TESTFWDIR = ./source/test_framework

# Compiling the compiler
compiler : clean_compiler lexer parser combine_no_gtest

# Compiling the gtest framework
gtest : clean_gtest lexer parser combine_gtest_gen_asm gtest_gen_asm combine_gtest

# Compile the lexer
lexer :
	flex -o $(FRONTDIR)/lexer.cpp $(FRONTDIR)/lexer.l

# Compile the parser
# Add -v flag to generate file containing extensive descriptions of parser and grammar.
parser :
	bison -v --defines=./source/headers/parser.hpp -o $(FRONTDIR)/parser.cpp $(FRONTDIR)/parser.y

# Compiling and combining the remaining needed source files for the Star compiler
combine_no_gtest :
	g++ -g -Wall -o star_compiler $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp \
	$(MAINDIR)/compiler_main.cpp $(MAINDIR)/compile_source_file.cpp  \
	$(filter-out $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp, $(wildcard $(FRONTDIR)/*.cpp)) \
	$(wildcard $(MIDDIR)/*.cpp) \
	$(wildcard $(BACKDIR)/*.cpp) \
	$(wildcard $(GLOBALDIR)/*.cpp)

# Compile and combining the compiled x86_64 ASM Star source test files together with the
#  Gtest framework, and the remaining source files.
combine_gtest :
	g++ $(TESTFWDIR)/gtest_main.cpp -lgtest -lpthread -o google_test -g -Wall -o run_gtest \
	$(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp \
	$(MAINDIR)/compile_source_file.cpp \
	$(filter-out $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp $(FRONTDIR)/compiler_main.cpp, $(wildcard $(FRONTDIR)/*.cpp)) \
	$(wildcard $(MIDDIR)/*.cpp) \
	$(wildcard $(BACKDIR)/*.cpp) \
	$(wildcard $(GLOBALDIR)/*.cpp) \
	$(wildcard $(GTESTASMDIR)/*.s)

# Compiling the compiler and creating a binary which will compile test Star source files into x86_64 ASM
combine_gtest_gen_asm :
	g++ $(TESTFWDIR)/gtest_asmfilegen.cpp -g -Wall -o gen_asm_files $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp \
	$(MAINDIR)/compile_source_file.cpp \
	$(filter-out $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp $(FRONTDIR)/compiler_main.cpp, $(wildcard $(FRONTDIR)/*.cpp)) \
	$(wildcard $(MIDDIR)/*.cpp) \
	$(wildcard $(BACKDIR)/*.cpp) \
	$(wildcard $(GLOBALDIR)/*.cpp)

# Using compiled compiler to compile given source Star programs into x86_64 ASM.
gtest_gen_asm :
	./gen_asm_files

# Remove the generated files from the gtest framework
clean_gtest:
	$(RM) $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp run_gtest gen_asm_files $(HEADERSDIR)/parser.hpp \
	$(FRONTDIR)/parser.output \
	$(RM) $(GTESTASMDIR)/*.s \

# Remove the generated files from the compiler
clean_compiler:
	$(RM) $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp star_compiler $(HEADERSDIR)/parser.hpp \
	$(FRONTDIR)/parser.output
	# $(RM) ./asm_out/*

# Remove the generated files from the compiler and the gtest framework
clean:
	$(RM) $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp star_compiler run_gtest gen_asm_files $(HEADERSDIR)/parser.hpp \
	$(FRONTDIR)/parser.output \
	$(RM) $(GTESTASMDIR)/*.s
	# $(RM) ./asm_out/*


.PHONY: clean

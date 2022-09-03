# Makefile where the source files from the source directory are compiled.

# Run "make gtest; ./run_gtest" to compile and run the test framework.

# Use "make compiler; ./star_compiler ./path-to-your-file" to compile the star compiler
# and use the star compiler to compile your given file. The generated assembly file is
# placed in the asm_out folder.

MAINDIR = ./source/main
FRONTDIR = ./source/front-end
MIDDIR = ./source/middle-end
BACKDIR = ./source/back-end
GTESTASMDIR = ./source/test_framework/test_files/gen_asm_files
GLOBALDIR = ./source/global
HEADERSDIR = ./source/headers
TESTFWDIR = ./source/test_framework

compiler : clean_compiler lexer parser combine_no_gtest

gtest : clean_gtest lexer parser combine_gtest_gen_asm gtest_gen_asm combine_gtest

lexer :
	flex -o $(FRONTDIR)/lexer.cpp $(FRONTDIR)/lexer.l

# Add -v flag to generate file containing extensive descriptions of parser and grammar.
parser :
	bison -v --defines=./source/headers/parser.hpp -o $(FRONTDIR)/parser.cpp $(FRONTDIR)/parser.y

combine_no_gtest :
	g++ -g -Wall -o star_compiler $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp \
	$(MAINDIR)/entry.cpp \
	$(filter-out $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp, $(wildcard $(FRONTDIR)/*.cpp)) \
	$(wildcard $(MIDDIR)/*.cpp) \
	$(wildcard $(BACKDIR)/*.cpp) \
	$(wildcard $(GLOBALDIR)/*.cpp)

combine_gtest :
	g++ $(TESTFWDIR)/gtest_main.cpp -lgtest -lpthread -o google_test -g -Wall -o run_gtest \
	$(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp \
	$(MAINDIR)/entry.cpp \
	$(filter-out $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp $(FRONTDIR)/main.cpp, $(wildcard $(FRONTDIR)/*.cpp)) \
	$(wildcard $(MIDDIR)/*.cpp) \
	$(wildcard $(BACKDIR)/*.cpp) \
	$(wildcard $(GLOBALDIR)/*.cpp) \
	$(wildcard $(GTESTASMDIR)/*.s)
	$(RM) gen_asm_files

combine_gtest_gen_asm :
	g++ $(TESTFWDIR)/gtest_asmfilegen.cpp -g -Wall -o gen_asm_files $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp \
	$(MAINDIR)/entry.cpp \
	$(filter-out $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp $(FRONTDIR)/main.cpp, $(wildcard $(FRONTDIR)/*.cpp)) \
	$(wildcard $(MIDDIR)/*.cpp) \
	$(wildcard $(BACKDIR)/*.cpp) \
	$(wildcard $(GLOBALDIR)/*.cpp)

gtest_gen_asm :
	./gen_asm_files

# Remove the generated binaries from the gtest framework
clean_gtest:
	$(RM) $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp run_gtest gen_asm_files $(HEADERSDIR)/parser.hpp \
	$(FRONTDIR)/parser.output \

# Remove the generated binaries from the compiler
clean_compiler:
	$(RM) $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp star_compiler $(HEADERSDIR)/parser.hpp \
	$(FRONTDIR)/parser.output \

# Remove the generated binaries from the compiler and the gtest framework
clean:
	$(RM) $(FRONTDIR)/lexer.cpp $(FRONTDIR)/parser.cpp star_compiler run_gtest gen_asm_files $(HEADERSDIR)/parser.hpp \
	$(FRONTDIR)/parser.output \

.PHONY: clean

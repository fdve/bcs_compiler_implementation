LEX=flex

all: lex.yy.c parser combined

lex.yy.c: lexer.l
	$(LEX) $(LFLAGS) -o $@ $^

parser : parser.y
	bison -d parser.y

combined : lex.yy.c parser.tab.c
	g++ -o lex_and_parse.out lex.yy.c parser.tab.c




clean:
	$(RM) *.c *.cc *.out *.h





#
# all: lex.yy.c lex.out
#
# lex.yy.c: calc.l
# 	$(LEX) $(LFLAGS) -o $@ $^
#
# lex.out : lex.yy.c
# 	c++ -o flex.out lex.yy.c
#
# clean:
# 	$(RM) *.c *.cc *.out *.h

LEX=flex

all: lex.yy.c lex.out

lex.yy.c: calc.l
	$(LEX) $(LFLAGS) -o $@ $^

lex.out : lex.yy.c
	c++ -o flex.out lex.yy.c

clean:
	$(RM) *.c *.cc *.out *.h

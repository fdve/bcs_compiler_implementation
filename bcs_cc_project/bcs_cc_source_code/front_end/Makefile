LEX=flex

all: lex.yy.c lex.out

lex.yy.c: flex_lexical_analyzer.l
	$(LEX) $(LFLAGS) -o $@ $^

lex.out : lex.yy.c
	gcc -o flex.out lex.yy.c

clean:
	$(RM) *.c *.out




# a.out:;


# lex.out : lex.yy.c
	# $(GCC) $(CFLAGS) $? -o a.out $@

# flex_lexical_analyzer: flex_lexical_analyzer.l
	# $(LEX) $(CFLAGS) $? lex.yy.c

	# $(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

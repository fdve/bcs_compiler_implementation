all: assembly

assembly : temp.s
	gcc -no-pie -o -g assembly.out temp.s

assembly_no_print : temp.s
	gcc -o -d assembly.out temp.s

clean:
	$(RM) *.out

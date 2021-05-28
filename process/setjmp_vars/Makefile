
.PHONY: all clean


all: optasm asm

noflags:
	cc -o setjmp_vars setjmp_vars.c

optimized:
	cc -O -o setjmp_vars setjmp_vars.c

optasm:
	cc -O -S setjmp_vars.c 
	mv ./setjmp_vars.s ./setjmp_varsOPT.s

asm:
	cc -S setjmp_vars.c



clean:
	rm -f setjmp_vars *.s

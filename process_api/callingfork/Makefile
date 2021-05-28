CC=cc
CFLAGS=-Wall -O -g 

.PHONY: all clean compare 

all: callingfork asmOPT asm

asm: callingfork.c
	$(CC) -S callingfork.c 

asmOPT: callingfork.c
	$(CC) $(CFLAGS) -S callingfork.c
	mv ./callingfork.s ./cforkOPT.s

compare: 
	vim -d callingfork.s cforkOPT.s

callingfork: callingfork.c
	$(CC) $(CFLAGS) callingfork.c -o callingfork

clean:
	rm -f callingfork *.s

CC=gcc
CFLAGS=-Wall -g -O

.PHONY: all clean

EXE=callfork2
SOURCES=callfork2.c

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -f $(EXE)

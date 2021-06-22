CC=gcc
CFLAGS=-Wall -g -O

.PHONY: all clean

EXE=wgrep
SOURCES=wgrep.c

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -rf $(EXE) tests-out

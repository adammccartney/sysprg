CC=gcc
CFLAGS=-Wall -g -O

.PHONY: all clean

EXE=wunzip
SOURCES=wunzip.c

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -rf $(EXE) tests-out

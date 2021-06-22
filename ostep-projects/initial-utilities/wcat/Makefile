CC=gcc
CFLAGS=-Wall -g -O

.PHONY: all clean

EXE=wcat
SOURCES=wcat.c

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -f $(EXE)

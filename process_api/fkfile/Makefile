CC=gcc
CFLAGS=-Wall -g -O

.PHONY: all clean

EXE=fkfile
SOURCES=fkfile.c

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -f $(EXE)

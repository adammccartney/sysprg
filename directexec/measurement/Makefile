CC=gcc
CFLAGS=-Wall -g -O

.PHONY: clean

EXE=measure
SOURCES=measure.c
ERRLOG=error.log
IO=

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -f $(EXE) $(ERRLOG) $(IO)

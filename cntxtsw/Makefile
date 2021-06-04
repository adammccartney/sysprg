CC=gcc
CFLAGS=-Wall -g -O

.PHONY: clean

EXE=cntxtsw
SOURCES=cntxtsw.c
ERRLOG=error.log
IO=

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -f $(EXE) $(ERRLOG) $(IO)

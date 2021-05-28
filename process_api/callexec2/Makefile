CC=gcc
CFLAGS=-Wall -g -O

.PHONY: all clean

EXE=callexec2
SOURCES=callexec2.c
ERRLOG=error.log

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

clean:
	rm -f $(EXE) $(ERRLOG)

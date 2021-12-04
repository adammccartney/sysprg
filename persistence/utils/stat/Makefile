SHELL := bash

CC := clang
CFLAGS := -Wall

EXE := stat
SOURCES := stat.c

all: $(EXE)

$(EXE): $(SOURCES)
	$(CC) $< -o $@

clean:
	rm $(EXE) error.log

.PHONY: all clean






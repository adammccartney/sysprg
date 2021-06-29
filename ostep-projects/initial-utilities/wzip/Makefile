CC=clang
CFLAGS=-Wall -g -O

.PHONY: all clean utestbyout

EXE=wzip
SOURCES=wzip.c

UTESTDIR=./tests/utest
TBOUTEXE=$(UTESTDIR)/test_byteout
TBOUTSRC=$(UTESTDIR)/test_byteout.c

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) 

utestbyout: $(TBOUTSRC)
	$(CC) $(CFLAGS) -o $(TBOUTEXE) $(TBOUTSRC)

check: $(SOURCES)
	clang-tidy -checks=* --warnings-as-errors=* $(SOURCES) > lint.log 2>&1


clean:
	rm -rf $(EXE) tests-out $(TBOUTEXE)

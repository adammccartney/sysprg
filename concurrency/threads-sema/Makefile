
TARGETS := barrier reader-writer reader-writer-nostarve

all: barrier reader-writer reader-writer-nostarve

barrier: 
	gcc -o barrier barrier.c -Wall -pthread

reader-writer:
	gcc -o reader-writer reader-writer.c -Wall -pthread

reader-writer-nostarve:
	gcc -o reader-writer-nostarve reader-writer-nostarve.c -Wall -pthread

clean:
	rm $(TARGETS) 


HEADERS:=common.h
EXE:=client 
EXE+=server

all: $(EXE)

client: TCPclient.c $(HEADERS)
	gcc -o client TCPclient.c -Wall

server: TCPserver.c $(HEADERS)
	gcc -o server TCPserver.c -Wall

clean:
	rm -rf $(EXE) error.log

.PHONY: clean

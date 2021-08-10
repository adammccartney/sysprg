
HEADERS := common.h

all: client server

client: TCPclient.c $(HEADERS)
	gcc -o client TCPclient.c -Wall

server: TCPserver.c $(HEADERS)
	gcc -o server TCPserver.c -Wall

clean:
	rm -rf $(all)

.PHONY: clean

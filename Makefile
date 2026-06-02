CC = gcc
CFLAGS = -Wall -I./include

all: bin/server bin/client

bin/server: src/server.c src/crypto.c
	$(CC) $(CFLAGS) -o bin/server src/server.c src/crypto.c

bin/client: src/client.c src/crypto.c
	$(CC) $(CFLAGS) -o bin/client src/client.c src/crypto.c

clean:
	rm -f bin/server bin/client

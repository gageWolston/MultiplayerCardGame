CC = gcc
CFLAGS = -Wall -Wextra -g

all: server client

server:
        $(CC) $(CFLAGS) -o server server.c Player.c GameManager.c Cards.c

client:
        $(CC) $(CFLAGS) -o client client.c

clean:
        rm -f server client *.o

CFLAGS=-c -g -Wall -Werror
CC=gcc

all:server client

recv_signal: client.o
	$(CC) -o client client.o

send_signal: server.o
	$(CC) -o server server.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o server client
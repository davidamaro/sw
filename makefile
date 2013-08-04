CC=gcc
CFLAGS=-lncurses
all:
	$(CC) sw.c $(CFLAGS) -Wall -Wextra -O2 -o sw
soft:
	$(CC) sw.c $(CFLAGS) -o sw

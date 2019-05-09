CC = gcc
CFLAGS = -Wall -std=c99

all: graph

graph.o: graph.c graph.h 
	$(CC) $(CFLAGS) -c graph.c

clean:
	rm *.o graph

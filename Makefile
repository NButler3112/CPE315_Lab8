CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
MAIN = matmul2
OBJS = matmul2.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

matmul2.o : matmul2.c matmul2.h
	$(CC) $(CFLAGS) -c matmul2.c

clean :
	rm $(MAIN) $(OBJS)


# Makefile for ls-v1.0.0
CC = gcc
CFLAGS = -Wall -g
SRC = src/ls-v1.0.0.c
OBJ = obj/ls-v1.0.0.o
BIN = bin/ls

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(BIN)


# Build v1.2.0 (column + long listing)
ls-v1.2.0: src/ls-v1.2.0.c
	$(CC) $(CFLAGS) src/ls-v1.2.0.c -o bin/ls

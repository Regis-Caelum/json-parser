CC=gcc
CFLAGS=-g -Wall -fdiagnostics-color=always

SRC = main.c $(wildcard commands/json/*.c) $(wildcard utility/*.c)

OBJ = $(SRC:.c=.o)

cli: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) cli

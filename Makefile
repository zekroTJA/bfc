CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = dist/bfc

$(TARGET): $(OBJ)
	mkdir -p $(dir $(TARGET))
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(dir $(TARGET))

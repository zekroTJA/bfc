CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = dist/bfc

$(TARGET): $(OBJ)
	mkdir -p $(dir $(TARGET))
	$(CC) $(OBJ) -o $(TARGET)

static: $(OBJ)
	mkdir -p $(dir $(TARGET))
	$(CC) -static $(OBJ) -o $(TARGET)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build dist

.PHONY: clean static

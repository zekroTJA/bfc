CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = $(CURDIR)/dist/bfc
TESTS_DIR = $(CURDIR)/tests


.PHONY: clean static test_deps


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

test_deps:
	python3 -m pip install --requirement $(TESTS_DIR)/requirements.txt

test: $(TARGET) | test_deps
	python3 $(TESTS_DIR)/run.py

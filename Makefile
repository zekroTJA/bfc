CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -fPIE
LDFLAGS = -pie
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = $(CURDIR)/dist/bfc
TESTS_DIR = $(CURDIR)/tests
FUZZ_IMAGE = bfc-fuzz
FUZZ_ARGS = -P explore
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

DOCKER := $(shell command -v docker 2>/dev/null)
ifeq ($(DOCKER),)
    DOCKER := $(shell command -v podman 2>/dev/null)
endif


.PHONY: clean static test build_fuzz fuzz


$(TARGET): $(OBJ)
	mkdir -p $(dir $(TARGET))
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET)

static: $(OBJ)
	mkdir -p $(dir $(TARGET))
	$(CC) -static $(OBJ) -o $(TARGET)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build dist

$(TESTS_DIR)/.deps: $(TESTS_DIR)/requirements.txt
	python3 -m pip install --requirement $(TESTS_DIR)/requirements.txt
	touch $(TESTS_DIR)/.deps

test: $(TARGET) | $(TESTS_DIR)/.deps
	python3 $(TESTS_DIR)/run.py

build_fuzz:
	$(DOCKER) build . -f "$(CURDIR)/fuzz/Dockerfile" -t $(FUZZ_IMAGE)

fuzz: build_fuzz
	mkdir -p $(CURDIR)/fuzz/out
	$(DOCKER) run -v "$(CURDIR)/fuzz/out:/output" $(FUZZ_IMAGE) $(FUZZ_ARGS)

valgrind: $(TARGET)
	$(VALGRIND) $(TARGET) inputs/hello_world.bf 1>/dev/null
	$(VALGRIND) $(TARGET) inputs/hello_world.bf --buffer-size 2 --dynamic-reallocation 1>/dev/null
	$(VALGRIND) $(TARGET) inputs/hello_world.bf --json 1>/dev/null
	$(VALGRIND) $(TARGET) inputs/hello_world.bf --buffer-size 2 --dynamic-reallocation --json 1>/dev/null

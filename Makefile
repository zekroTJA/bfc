CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -fPIE
LDFLAGS = -pie
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = $(CURDIR)/dist/bfc
TESTS_DIR = $(CURDIR)/tests
FUZZ_IMAGE = bfc-fuzz
FUZZ_ARGS = -P explore


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
	docker build . -f "$(CURDIR)/fuzz/Dockerfile" -t $(FUZZ_IMAGE)

fuzz: build_fuzz
	docker run -v "$(CURDIR)/fuzz/out:/output" $(FUZZ_IMAGE) $(FUZZ_ARGS)

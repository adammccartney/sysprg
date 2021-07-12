CC := clang
CFLAGS := -c -g -Wall -O
SRC_DIR := src
BUILD_DIR := build
SOURCES := $(SRC_DIR)/null.c

all: pre_setup format $(BUILD_DIR)/null

$(BUILD_DIR)/null: $(BUILD_DIR)/null.o 
	$(CC) $^ -o $@

$(BUILD_DIR)/null.o: $(SRC_DIR)/null.c
	$(CC) $(CFLAGS) $< -o $@

format:
	find . -type f -name "*.[c|h]" | xargs clang-format -i

pre_setup:
	mkdir -p $(BUILD_DIR)

check: $(SOURCES)
	clang-tidy -checks=* --warnings-as-errors=* $(SOURCES) > lint.log 2>&1


clean:
	rm -rf $(BUILD_DIR)

# Run cdecl
run:
	./$(BUILD_DIR)/null

.PHONY: pre_setup

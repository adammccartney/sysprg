CC := clang
CFLAGS := -c -g -Wall -O
SRC_DIR := src
BUILD_DIR := build
SOURCES := $(SRC_DIR)/vmseg.c

all: pre_setup format $(BUILD_DIR)/vmseg $(BUILD_DIR)/diss.asm

$(BUILD_DIR)/vmseg: $(BUILD_DIR)/vmseg.o 
	$(CC) $^ -o $@

$(BUILD_DIR)/vmseg.o: $(SRC_DIR)/vmseg.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/diss.asm: $(BUILD_DIR)/vmseg $(BUILD_DIR)/vmseg.o
	objdump -d $(BUILD_DIR)/vmseg > $(BUILD_DIR)/exe.asm
	objdump -d $(BUILD_DIR)/vmseg.o > $(BUILD_DIR)/o.asm

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
	./$(BUILD_DIR)/vmseg

.PHONY: pre_setup format

# Makefile for building a single configuration of wien shell. It expects
# variables to be passed in for:
#
# MODE         "debug" or "release".
# NAME         Name of the output executable (and object file directory).
# SOURCE_DIR   Directory where source files and headers are found.
#
# c.make courtesy of Bob Nystrom

ifeq ($(CPP), true)
	CFLAGS := -std=c++11
	C_LANG := -x c++
else
	CFLAGS := -std=c99
endif

CFLAGS += -Wall -Wextra -Werror -Wno-unused-parameter 
SRC_DIR := src

# Mode configuration
ifeq ($(MODE),debug)
	CFLAGS += -O0 -DDEBUG -g
	BUILD_DIR := build/debug
else
	CFLAGS += -O3 -flto
	BUILD_DIR := build/release
endif

# Files
HEADERS := $(wildcard $(SRC_DIR)/*.h)
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(addprefix $(BUILD_DIR)/$(NAME)/, $(notdir $(SOURCES:.c=.o)))


# Targets ----------------------------------------------------------------

# Link the shell intepreter
build/$(NAME): $(OBJECTS)
	@ printf "%8s %-40s %s\n" $(CC) $@ "$(CFLAGS)"
	@ mkdir -p build
	@ $(CC) $(CFLAGS) $^ -o $@

# Compile the object files
$(BUILD_DIR)/$(NAME)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ printf "%8s %-40s %s\n" $(CC) $< "$(CFLAGS)"
	@ mkdir -p $(BUILD_DIR)/$(NAME)
	@ $(CC) -c $(C_LANG) $(CFLAGS) -o $@ $<

.PHONY: default



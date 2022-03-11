SHELL := /usr/bin/bash

CC := gcc

BUILD_DIR := build

default: wish 

# Compiule a debug build of wish
debug:
	@ $(MAKE) -f ./c.make NAME=wishd MODE=debug SOURCE_DIR=src

wish:
	@ $(MAKE) -f ./c.make NAME=wish MODE=release SOURCE_DIR=src

clean:
	rm -rf build

test: debug
	@ ./test-wish.sh 

# Linting stuff 
format:
	find . -type f -name "*.[c|h]" | xargs clang-format -i

check: src/*
	clang-tidy -checks=* --warnings-as-errors=* src/* > lint.log 2>&1

.PHONY: check clean wish debug format test 

#
# Makefile for skod.
# 
# Copyright (c) 2015 by Hypsurus <hypsurus@mail.ru>
#
# This file is part of skod project
#  

# Compiler and linker
# on gnu/linux it will be symlink to gcc
CC=cc

# Version
PACKAGE_VERSION=1.0

# C flags
CFLAGS = -ggdb -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\" -Wall -Werror

# Sources
SRC_DIR=src
PREFIX=/usr
HEADERS_SOURCES=$(wildcard $(SRC_DIR)/*.h)
SOURCES=$(wildcard $(SRC_DIR)/*.c)
BIN=skod
BC_LOCATION=share/bash-completion/completions/

all: skod

# Compile
skod:
	$(CC) $(CFLAGS) $(SOURCES) -o $@ $^

# Install skod
install: all
	cp ${SRC_DIR}/${BIN}.bash_completion ${PREFIX}/${BC_LOCATION}/skod
	install -Dm775 skod ${PREFIX}/bin/skod

# Remove skod
uninstall:
	rm -f ${BC_LOCATION}/skod
	rm -f ${PREFIX}/bin/skod

# Enable debug
debug: CFLAGS += -DDEBUG
debug: skod

clean:
	rm -f skod

.PHONY: all skod

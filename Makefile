#
# Makefile for skod.
# 
#
# Copyright (c) 2015,2016 by Hypsurus <hypsurus@mail.ru>
#
# This file is part of the skod project
#  
# Compiler and linker
# on gnu/linux it will be symlink to gcc
#
CC=cc

# Version
PACKAGE_VERSION=1.0

# C flags
CFLAGS = -ggdb -s -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\" -Wall -Werror

# Sources
SRC_DIR=src
PREFIX=/usr
HEADERS_SOURCES=$(wildcard $(SRC_DIR)/*.h)
SOURCES=$(wildcard $(SRC_DIR)/*.c)

all: skod

# Compile
skod:
	$(CC) $(CFLAGS) $(SOURCES) -o $@ $^

# Install skod
install: all
	install -Dm775 skod ${PREFIX}/bin/skod

# Remove skod
uninstall:
	rm -f ${PREFIX}/bin/skod

# Enable debug
debug: CFLAGS += -DDEBUG
debug: skod

clean:
	rm -f skod

.PHONY: all skod

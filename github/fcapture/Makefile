CC = gcc
INCLUDE = -I$(CURDIR)/util
CFLAGS = -Wall -Werror -O2 -ggdb $(INCLUDE)
LDFLAGS = -lpcap -lJudy

PREFIX = /usr
VERSION := 0.0.1
BUILDTIME := $(shell date -u)

ifneq ($(shell uname), Linux)
PREFIX = /usr/local
INCLUDE += -I/usr/local/include
LDFLAGS += -L/usr/local/lib
endif

BIN = bin/fcapture bin/fcapdump

UTIL = \
	util/version.o \
	util/util.o \
	util/pcapnet.o \
	util/mmap.o\

FCAPTURE = \
	src/fcapture.o \
	src/fcap.o \
	src/flow.o

FCAPDUMP = \
	src/fcapdump.o

ALL = $(BIN) $(UTIL) $(FCAPTURE) $(FCAPDUMP)

all: $(ALL)
	@-rm -f util/version.o

bin/fcapture: $(UTIL) $(FCAPTURE)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bin/fcapdump: $(FCAPDUMP) util/version.o util/util.o src/fcap.o util/mmap.o
	$(CC) $(CFLAGS) -o $@ $^

util/version.o:
	$(CC) -DBUILDTIME="$(BUILDTIME)" -DVERSION="$(VERSION)" -o $@ -c util/version.c

clean:
	rm -f $(ALL)

install: $(BIN)
	install -m 0755 $(BIN) $(DESTDIR)$(PREFIX)/bin

.PHONY: clean install

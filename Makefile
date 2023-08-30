# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/afeirsa/*.h)
OBJECTS = $(SOURCES:.c=.o)

ifndef APPLE
	GLXABI = 1
endif

OUT = src/libafeirsa.a

XQUARTZ_ROOT = /opt/X11
ifdef GLXABI
	CFLAGS += -DGLXABI
	LDFLAGS += -lGL

	ifdef APPLE
		LDFLAGS += -L$(XQUARTZ_ROOT)/lib
		CFLAGS += -I$(XQUARTZ_ROOT)/include
	endif
else
	ifdef APPLE
		CFLAGS += -DNSGL
		LDFLAGS += -framework OpenGL
	endif
endif

VERSION = $(shell cat VERSION)

CFLAGS += -Iinclude -DVERSION=$(VERSION) -DAF_BUILD
CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors

ifndef NO_STDLIB
	CFLAGS += -DUSE_STDLIB
	LDFLAGS += -lc
endif

ifdef GL10_COMPAT
	CFLAGS += -DGL10_COMPAT
endif

ifdef NO_EXT
	CFLAGS += -DNO_EXT
endif

ifdef DEBUG
	CFLAGS += -g -O0 -D_DEBUG
else
	CFLAGS += -Ofast -DNDEBUG
endif

PREFIX = /usr/local

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUT)

$(OUT): $(OUT)($(OBJECTS))

$(OBJECTS): $(HEADERS)

.PHONY: install
install: PCTMP := $(shell mktemp)
install: $(OUT)
ifdef DEBUG
	@echo "warning: installing debug binaries!"
else
	strip -x $(OUT)
endif

	install -d $(PREFIX)/include/afeirsa
	install -d $(PREFIX)/lib/pkgconfig
	install -d $(PREFIX)/share/man/man3

	install $(OUT) $(PREFIX)/lib

	install $(HEADERS) $(PREFIX)/include/afeirsa
	install $(wildcard doc/*.3) $(PREFIX)/share/man/man3

	echo "prefix=$(PREFIX)" > $(PCTMP)
	echo "version=$(VERSION)" >> $(PCTMP)
	cat build/afeirsa.pc.in >> $(PCTMP)
	install $(PCTMP) $(PREFIX)/lib/pkgconfig/afeirsa.pc

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)

ifdef BUILD_EXAMPLES
include examples/examples.mk
endif

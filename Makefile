# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

(%): %
%.a:
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/afeirsa/*.h)
OBJECTS = $(SOURCES:.c=.o)

ifdef STDCC
	ifdef APPLE
		# AF_NSGL requires non-compliant `cc' shenanigans
		GLXABI = 1
	endif
endif

ifndef APPLE
	GLXABI = 1
endif

OUT = src/libafeirsa.a

XQUARTZ_ROOT = /opt/X11
ifdef GLXABI
	GLABI += -DAF_GLXABI
	LDFLAGS += -lGL -lglut -lGLU

	ifdef APPLE
		LDFLAGS += -L$(XQUARTZ_ROOT)/lib
		PUBLIC_IFLAGS += -I$(XQUARTZ_ROOT)/include
	endif
else
	ifdef APPLE
		GLABI += -DAF_NSGL
		LDFLAGS += -framework GLUT -framework OpenGL
	endif
endif

VERSION = $(shell cat VERSION)

CFLAGS += -Iinclude -DVERSION=$(VERSION) -DAF_BUILD $(GLABI) $(PUBLIC_IFLAGS)
ifndef STDCC
	CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors
endif

ifndef NO_STDLIB
	CFLAGS += -DUSE_STDLIB
endif

ifdef GL10_COMPAT
	CFLAGS += -DGL10_COMPAT
endif

ifdef NO_EXT
	CFLAGS += -DNO_EXT
endif

ifdef DEBUG
	CFLAGS += -g -D_DEBUG
	ifndef STDCC
		CFLAGS += -O0
	endif
else
	CFLAGS += -DNDEBUG
	ifndef STDCC
		CFLAGS += -Ofast
	else
		CFLAGS += -O
	endif
endif

PREFIX = /usr/local

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUT)

$(OUT): $(OBJECTS)

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
	echo "ldflags=$(LDFLAGS)" >> $(PCTMP)
	echo "glabi=$(GLABI)" >> $(PCTMP)
	echo "iflags=$(PUBLIC_IFLAGS)" >> $(PCTMP)
	cat build/afeirsa.pc.in >> $(PCTMP)
	install $(PCTMP) $(PREFIX)/lib/pkgconfig/afeirsa.pc

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)

ifdef BUILD_EXAMPLES
include examples/examples.mk
endif

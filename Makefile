# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

ifndef RANLIB
	RANLIB = ranlib
endif

ifdef WINDOWS_SHELL
	RM = del
define PATHREM
	$(RM) $(subst /,\\,$(1))
endef
else
define PATHREM
	$(RM) $(1)
endef
endif

(%): %
%.a:
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

include build/glabi.mk

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/afeirsa/*.h)
OBJECTS = $(SOURCES:.c=.o)

ifdef STDCC
	ifdef APPLE
		# AF_NSGL requires non-compliant `cc' shenanigans
		GLXABI = 1
	endif
endif

OUT = src/libafeirsa.a

CFLAGS += -Iinclude -DAF_BUILD $(GLABI) $(PUBLIC_IFLAGS)
ifndef STDCC
	CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors
endif

ifdef WINDOWS
	CFLAGS += -D_WINDOWS -Wno-long-long
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
else
	CFLAGS += -DNDEBUG -O
endif

PREFIX = /usr/local

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUT)

$(OUT): $(OBJECTS)

$(OBJECTS): $(HEADERS)

ifndef WINDOWS
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
	echo "version=$(shell cat VERSION)" >> $(PCTMP)
	echo "ldflags=$(LDLIBS)" >> $(PCTMP)
	echo "glabi=$(GLABI)" >> $(PCTMP)
	echo "iflags=$(PUBLIC_IFLAGS)" >> $(PCTMP)
	cat build/afeirsa.pc.in >> $(PCTMP)
	install $(PCTMP) $(PREFIX)/lib/pkgconfig/afeirsa.pc
endif

.PHONY: clean
clean:
	$(call PATHREM,$(OBJECTS))
	$(call PATHREM,$(OUT))

ifdef BUILD_EXAMPLES
include examples/examples.mk
endif

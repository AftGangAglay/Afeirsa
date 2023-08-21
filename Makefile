SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/afeirsa/*.h)
OBJECTS = $(SOURCES:.c=.o)

OUT = src/libafeirsa.a

CFLAGS += -Iinclude
CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors
ifdef USE_STDLIB
CFLAGS += -DUSE_STDLIB
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
$(OUT): $(OBJECTS)
	$(AR) rcs $@ $^

$(OBJECTS): $(HEADERS)

.PHONY: install
install: $(OUT)
	install -d $(PREFIX)/include/afeirsa
	install -d $(PREFIX)/lib/pkgconfig
	install -d $(PREFIX)/share/man/man3

	install $(OUT) $(PREFIX)/lib

	install $(HEADERS) $(PREFIX)/include/afeirsa
	install $(wildcard doc/*.3) $(PREFIX)/share/man/man3

	echo "prefix=$(PREFIX)" > /tmp/afeirsa.pc
	cat build/afeirsa.pc.in >> /tmp/afeirsa.pc
	install /tmp/afeirsa.pc $(PREFIX)/lib/pkgconfig/

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)

ifdef BUILD_EXAMPLES
include examples/examples.mk
endif

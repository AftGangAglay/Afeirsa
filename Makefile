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
	-mkdir -p $(PREFIX)/include/afeirsa
	-mkdir -p $(PREFIX)/lib/pkgconfig
ifeq ($(shell uname -s),Darwin)
	install $(OUT) $(PREFIX)/lib
	strip -x $(OUT)
else
	install -s $(OUT) $(PREFIX)/lib
endif
	install $(HEADERS) $(PREFIX)/include/afeirsa

	echo "prefix=$(PREFIX)" > $(PREFIX)/lib/pkgconfig/afeirsa.pc
	cat build/afeirsa.pc.in >> $(PREFIX)/lib/pkgconfig/afeirsa.pc

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)

ifdef BUILD_EXAMPLES
include examples/examples.mk
endif

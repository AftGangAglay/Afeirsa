SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/afeirsa/*.h)
OBJECTS = $(SOURCES:.c=.o)

OUT = src/libafeirsa.a

CFLAGS += -Iinclude
CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors
ifdef USE_STDLIB
CFLAGS += -DUSE_STDLIB
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
ifeq ($(shell uname -s),Darwin)
	strip -x $(OUT)
	install $(OUT) $(PREFIX)/lib
	mkdir $(PREFIX)/afeirsa
	install $(HEADERS) $(PREFIX)/include/afeirsa
else
	install -s $(OUT) $(PREFIX)/lib
	install -D $(HEADERS) -t $(PREFIX)/include/afeirsa
endif

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(OUT)

ifdef BUILD_EXAMPLES
include examples/examples.mk
endif

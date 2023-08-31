# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

EXAMPLES_SOURCES = $(wildcard examples/*.c)
EXAMPLES_OBJECTS = $(EXAMPLES_SOURCES:.c=.o)
EXAMPLES = $(EXAMPLES_SOURCES:.c=)

EXAMPLES_CFLAGS := $(CFLAGS)
EXAMPLES_CFLAGS += $(shell pkg-config --cflags glfw3)

EXAMPLES_LDLIBS := $(LDFLAGS)
EXAMPLES_LDLIBS += $(OUT) $(shell pkg-config --libs glfw3) -lm

$(EXAMPLES): CFLAGS := $(EXAMPLES_CFLAGS)
$(EXAMPLES): LDLIBS := $(EXAMPLES_LDLIBS)
$(EXAMPLES): $(EXAMPLES_SOURCES) $(OUT)

all: examples

.PHONY: examples
examples: $(EXAMPLES)

clean: clean_examples

.PHONY: clean_examples
clean_examples:
	rm -rf $(EXAMPLES)

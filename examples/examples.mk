# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+afeirsa@pm.me>

EXAMPLES_SOURCES = $(wildcard examples/*.c)
EXAMPLES = $(EXAMPLES_SOURCES:.c=)

EXAMPLES_CFLAGS := $(CFLAGS)
EXAMPLES_CFLAGS += $(shell pkg-config --cflags glfw3)

EXAMPLES_LDFLAGS := $(LDFLAGS)
EXAMPLES_LDFLAGS += $(AFEIRSA_LIB) $(shell pkg-config --libs glfw3) -lm

UNAME = $(shell uname -s)
ifeq ($(UNAME),Darwin)
EXAMPLES_LDFLAGS += -framework OpenGL
else
EXAMPLES_LDFLAGS += -lGL
endif

$(EXAMPLES): CFLAGS := $(EXAMPLES_CFLAGS)
$(EXAMPLES): LDFLAGS := $(EXAMPLES_LDFLAGS)
$(EXAMPLES): $(EXAMPLES_SOURCES) $(OUT)

all: examples

.PHONY: examples
examples: $(EXAMPLES)

clean: clean_examples

.PHONY: clean_examples
clean_examples:
	rm -rf $(EXAMPLES)

# NOTE: This helps IDEs configure source paths correctly
ifndef EXAMPLES_DIR
EXAMPLES_DIR = .
endif

SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)

OUTS = $(SOURCES:.c=)

CFLAGS += -I$(EXAMPLES_DIR)/../include
CFLAGS += $(shell pkg-config --cflags glfw3)
CFLAGS += -std=c89 -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors

LDFLAGS += -L$(EXAMPLES_DIR)/../src -lafeirsa
LDFLAGS += $(shell pkg-config --libs glfw3)

UNAME = $(shell uname -s)
ifeq ($(UNAME),Darwin)
LDFLAGS += -framework OpenGL
else
LDFLAGS += -lGL
endif

.DEFAULT_GOAL := all
.PHONY: all
all: $(OUTS)

$(OUTS): $(SOURCES)

.PHONY: clean
clean:
	rm -rf $(OUTS)

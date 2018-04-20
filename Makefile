NAME = janpatch-cli

CC ?= gcc
CFLAGS ?= -Wall

MACROS += -DJANPATCH_STREAM=FILE
CFLAGS += -I. -std=gnu99

all: build

.PHONY: build clean

build:
	$(CC) $(MACROS) $(CFLAGS) cli/janpatch-cli.c -o $(NAME)

clean:
	rm $(NAME)

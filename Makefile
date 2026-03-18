CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11

SRC     = $(wildcard *.c)
BINS    = $(SRC:.c=)

all: $(BINS)

%: %.c darray.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)

.PHONY: all clean

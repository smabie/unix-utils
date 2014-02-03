# Makefile

# 
# This file is public domain as declared by Sturm Mabie.
# 

CC=gcc
CFLAGS+=-Wall -ansi -pedantic
ifeq ($(shell uname),Linux)
	CFLAGS+=-D_BSD_SOURCE -D_POSIX_SOURCE -lbsd
endif
all:
	mkdir -p bin
	$(CC) $(CFLAGS) btee.c -o bin/btee
	$(CC) $(CFLAGS) lambda.c -o bin/lambda
	$(CC) $(CFLAGS) pick.c -o bin/pick
	$(CC) $(CFLAGS) slice.c -o bin/slice
clean:
	rm -rf bin


# Makefile

# 
# This file is public domain as declared by Sturm Mabie.
# 

CC=gcc
CFLAGS+=-Wall -ansi -pedantic

all:
	mkdir bin
	$(CC) $(CFLAGS) btee.c -o bin/btee
	$(CC) $(CFLAGS) lambda.c -o bin/lambda
	$(CC) $(CFLAGS) pick.c -o bin/pick
	$(CC) $(CFLAGS) slice.c -o bin/slice

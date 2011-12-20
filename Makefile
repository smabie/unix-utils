# Makefile

# 
# This file is public domain as declared by Sturm Mabie.
# 

CC=gcc
CFLAGS+=-Wall -ansi -pedantic -D_BSD_SOURCE

all:
	$(CC) $(CFLAGS) btee.c -o btee
	$(CC) $(CFLAGS) lambda.c -o lambda
	$(CC) $(CFLAGS) pick.c -o pick
	$(CC) $(CFLAGS) slice.c -o slice

target = cc
headers = main.h getopt.h  stdio.h stdlib.h string.h
sources = main.c getopt.c libc.c
cc = gcc
cflags = -Wall -pedantic -g -O -fno-builtin

all : $(target)

clean :
	rm -f $(target)

$(target) : $(headers) $(sources)
	$(cc) $(cflags) -o $(target) $(sources)

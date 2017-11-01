target = cc
headers = main.h getopt.h
sources = main.c getopt.c
cc = gcc
cflags = -ansi -Wall -pedantic -g

all : $(target)

clean :
	rm -f $(target)

$(target) : $(headers) $(sources)
	$(cc) $(cflags) -o $(target) $(sources)

target = as
headers =
sources = main.c
cc = gcc
cflags = -std=c89 -Wall -pedantic -O3

all : $(target)

clean :
	rm -f $(target)

$(target) : $(headers) $(sources)
	$(cc) $(cflags) -o $(target) $(sources)

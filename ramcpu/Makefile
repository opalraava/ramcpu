CXX=g++ -std=c++11 -g -O0
CC=cc -Wall -pedantic -ansi -g -O0

all : ramcpu as

clean :
	rm -f *~ *.o ramcpu as ramcpu++ as++ prog1 prog2 prog3

ramcpu : ramcpu.c ramcpu.h ramcpu_opcodes.h
	${CC} -o ramcpu ramcpu.c

as : as.c ramcpu.h ramcpu_opcodes.h as_util.h as_util.o
	${CC} -o as as.c as_util.o

as_util.o : as_util.h as_util.c
	${CC} -c -o as_util.o as_util.c

ramcpu++ : ramcpu++.cc
	${CXX} -o ramcpu++ ramcpu++.cc

as++ : as++.cc
	${CXX} -o as++ as++.cc

test : prog1 prog2 prog3
	rm -f prog1 prog2 prog3

prog1 : as++ prog1.s
	./as++ -o prog1 prog1.s

prog2 : as++ prog2.s
	./as++ -o prog2 prog2.s

prog3 : as++ prog3.s
	./as++ -o prog3 prog3.s

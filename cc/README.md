ramcpu minimal c compiler
=========================

To get this thing to compile itself it needs to be very
simple. so it has no floating point. It does not have
most of the standard libc. Just the basic things like:

* stdio.h, stdlib.h, string.h
* char, int, long, void, structs, unsigned, const, extern + static
* standard type declarations, definitions and operators,
  functions, flow control, block statements.
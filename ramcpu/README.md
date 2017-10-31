The ramcpu play project
=======================

The virtual machine implemented by ./ramcpu is very simple. It has
only two registers, 'a' and 'b'. Registers, addresses and opcodes are
all native integers (int). Basically everything is an int. There is
only one interrupt, divide by zero. After the execution of the 'stop'
instruction the process stops and ./ramcpu prints the value of the
two registers, the only output possible.

To build, just type 'make'.

To assemble a binary, just say:

    ./as -o binary source.s

To run it, say:

    ./ramcpu binary

or, with a bit of debugging info:

    DEBUG=true ./ramcpu binary

There are three sample programs that should give an impression of the
syntax understood by as, but it's generally AT&T syntax.

Assembly syntax
---------------

The assembler only knows how to use labels in jump instructions, not
for accessing memory addresses. A label is just an identifier followed
by a semicolumn. Examples of labels are:

    loop:
    exit:
    L0383:

Ok, now for the instructions.

* nop  [do nothing, opcode = 0]
* stop [terminates cpu execution loop]

* load $value, %a
* load $value, %b
* load ($addr), %a
* load ($addr), %b
* load %b, %a
* load %a, %b

* store %a, ($addr)
* store %b, ($addr)

* xchg  [exchange %a and %b]

* add %b, %a [a = a + b]
* add %a, %b [b = b + a]

* sub %b, %a [a = a - b]
* sub %a, %b [b = b - a]

* mul %b, %a [a = a * b]
* mul %a, %b [b = b * a]

* div %b, %a [ a = a/b; b = a%b; ]
* div %a, %b [ a = b/a; b = b%a; ]

Comparing sets boolean flags in the cpu:

    bool eq, ne, gt, lt, ge, le; // ==, !=, >, <, >=, <=

conditonal jumps are available for each of these flags.

* cmp $value, %a
* cmp ($addr), %a
* cmp %b, %a
* cmp $value, %b
* cmp ($addr), %b
* cmp %a, %b

Unconditinal jumps:

* jmp $value
* jmp ($addr)
* jmp label

Conditional jumps (see cmp instruction)

* jeq $value
* jeq ($addr)
* jeq label

* jne $value
* jne ($addr)
* jne label

* jgt $value
* jgt ($addr)
* jgt label

* jgt $value
* jgt ($addr)
* jgt label

* jge $value
* jge ($addr)
* jge label

* jle $value
* jle ($addr)
* jle label

* brk [get ram size into register %a]
* trap $num, $addr [set int <num> to jump to addr <addr>]
* int $num [execute interrupt $num]

That's all, folks!

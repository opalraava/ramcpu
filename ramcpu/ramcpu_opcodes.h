#ifndef INCLUDED_RAMCPU_OPCODES_H
#define INCLUDED_RAMCPU_OPCODES_H

/* opcode argument indicators */
#define DATA_TYPE_INT  0
#define DATA_TYPE_CHAR 1

#define ARG_TYPE_REG   2
#define ARG_TYPE_VALUE 3
#define ARG_TYPE_ADDR  4

/* opcodes */
#define OPCODE_NOP  0
#define OPCODE_STOP 1
#define OPCODE_LOAD 2

#endif /* INCLUDED_RAMCPU_OPCODES_H */

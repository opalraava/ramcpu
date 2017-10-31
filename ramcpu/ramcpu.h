#ifndef INCLUDED_RAMCPU_H
#define INCLUDED_RAMCPU_H

/* cpu characteristics */
#define CPU_NUM_CHAR_REGISTERS 10
#define CPU_NUM_INT_REGISTERS  10
#define CPU_SIZE_TRAP_VECTOR   1

/* default memory size */
#define RAM_DEFAULT_SIZE       1024

#define TRAP_DIVIDE_BY_ZERO    0
#define TRAP_INVALID_OPCODE    1
#define TRAP_NULL_REFERENCE    2

#endif /* INCLUDED_RAMCPU_H */

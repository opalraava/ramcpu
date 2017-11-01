#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
 * stdio.h
 */

int
printf(char* fmt, ...)
{
  return 0;
}

/*
 * stdlib.h
 */

void
exit(int status)
{
  /* exit system call */
  __asm__("movl $1,%eax;"
      "xorl %ebx,%ebx;"
      "int  $0x80"
      );
}

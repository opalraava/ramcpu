#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static void syscall_exit(int status);
static void syscall_write(int fileno, char* buf, int size);

static long _exit(int errcode);
static long _write(long fd, const char* buf, unsigned long len);

/*
 * stdio.h
 */

int
printf(char* fmt, ...)
{
  int size = strlen(fmt);
  /*  syscall_write(1,fmt,size); */
  _write(1,fmt,size);
  return 0;
}

/*
 * stdlib.h
 */

void
exit(int status)
{
  /* syscall_exit(status); */
  _exit(status);
}

/*
 * string.h
 */

int
strlen(char* s)
{
  int i = 0;
  while (*s++) i++;
  return i;
}








/* Linux System Calls */

/* 

 https://stackoverflow.com/questions/3866217/how-can-i-make-the-system-call-write-print-to-the-screen

 GCC INLINE ASM ==>
 https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html

system call number ==>
1 = exit
2 = fork
3 = read
4 = write
5 = open
6 = close

stdin=0
stdout=1
stderr=2
*/

static void
syscall_exit(int status)
{
  __asm__("movl %0,%%eax;"
	  "xorl %%ebx,%%ebx;"
	  "int  $0x80"
	  :
	  : "r" (status)
	  : "%eax", "%ebx"
	  );
}
static void
syscall_write(int fileno, char* buf, int size)
{
  __asm__("movl $4, %%eax;"
	  "movl %0, %%ebx;"
	  "movl %1, %%ecx;"
	  "movl %2, %%ecx;"
	  "movl %2, %%edx;"
	  "int $0x80"
	  :
	  : "r" (fileno), "m" (buf), "r" (size)
	  : "%eax", "%ebx", "%ecx", "%edx"
	  );
}


/*
 * https://github.com/lief-project/tutorials/blob/master/05_ELF_infect_plt-got/arch/x86_64/syscall.c
 */


static long _write(long fd, const char* buf, unsigned long len) {

  register long ret asm ("rax");
  register long _fd asm ("rdi") = fd;
  register const char* _buf asm ("rsi") = buf;
  register unsigned long _len asm ("rdx") = len;
  register int sys_write asm ("rax") = 1;
  asm volatile (
      "syscall;"
      : "=r" (ret)
      : "r" (_fd), "r" (_buf), "r" (_len), "r" (sys_write)
      :
  );
  return ret;
}

static long _exit(int errcode) {

  register long ret asm ("rax");
  register int _errcode asm ("rdi") = errcode;
  register int sys_exit asm ("rax") = 60;
  asm volatile (
      "syscall;"
      : "=r" (ret)
      : "r" (_errcode), "r" (sys_exit)
      :
  );
  return ret;
}

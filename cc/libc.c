#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static long syscall_exit(int errcode);
static long syscall_write(long fd, const char* buf, unsigned long len);

/*
 * stdio.h
 */

int
printf(char* fmt, ...)
{
  puts(fmt);
  return 0;
}

int
fputs(char* str, FILE* stream)
{
  int size = strlen(str);
  
  syscall_write(stream->fd,str,size);
  return 0;
}
int
puts(char* str)
{
  int size;
  char nl[] = "\r\n";
  
  size = strlen(str);
  syscall_write(1,str,size);
  size = strlen(nl);
  syscall_write(1,nl,size);
  return 0;
}

/*
 * stdlib.h
 */

void
exit(int status)
{
  syscall_exit(status);
}

/*
 * string.h
 */

int
strlen(char* s)
{
  char* t = s;
  
  while (*t++) ;
  return (t-s);
}









/* Linux System Calls */

# if 0
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
#endif

/*
 * https://github.com/lief-project/tutorials/blob/master/05_ELF_infect_plt-got/arch/x86_64/syscall.c
 */


static long syscall_write(long fd, const char* buf, unsigned long len) {

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

static long syscall_exit(int errcode) {

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

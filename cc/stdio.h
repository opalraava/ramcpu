#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#ifndef NULL
#define NULL 0
#endif
#define EOF (-1)

struct file_t {
  long fd;
};
typedef struct file_t FILE;

/* printf() */
int printf(char* fmt, ...);
int puts(char* str);
int fputs(char* str, FILE* stream);

#endif

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "as_util.h"

void
ltrim(char* str)
{
  char *s = str, *t;

  if (s == NULL || *s == '\0') return;

  while (*s && isspace(*s)) ++s;

  if (!*s) { str[0] = '\0'; return; }
  if (s == str) return;

  t = str;
  
  while (*s) {
    *t++ = *s++;
  }
  
  *t = '\0';
}

void rtrim(char* str)
{
  char* s = str;
  
  if (s == NULL || *s == '\0') return;
  
  while (*s) ++s;
  for (--s; isspace(*s); --s) {
    *s = '\0';
    if (s == str) break;
  }
}

char* dupstr(char* s)
{
  char* t;

  if (s == NULL) return NULL;
  
  t = malloc(strlen(s));
  return strcpy(t,s);
}

void
trim(char* str)
{
  rtrim(str);
  ltrim(str);
}


int
try_parse_identifier(char* pos, char** next_pos)
{
  char* s;
  
  if (pos == NULL || *pos == '\0' || next_pos == NULL) return 1;

  s = pos;
  
  if (isalpha(*s) || *s == '_' || *s == '.') {
    ++s;
    while (*s && (isalnum(*s) || *s == '_'))
      ++s;
    *next_pos = s;
  }
  else
    return 1;

  return 0;
}

int
try_parse_instruction(char* pos, char** next_pos)
{
  char* s;
  
  if (pos == NULL || *pos == '\0' || next_pos == NULL) return 1;

  s = pos;

  if (isalpha(*s) || *s == '.') {
    ++s;
    while (*s && (isalnum(*s) || *s == '_'))
      ++s;
      
    *next_pos = s;
    return 0;
  }
  
  return 1;
}

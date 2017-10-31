#ifndef INCLUDED_AS_UTIL_H
#define INCLUDED_AS_UTIL_H

void ltrim(char*);
void rtrim(char*);
void trim(char*);
char* dupstr(char*);
int try_parse_identifier(char* pos, char** next_pos); /* returns NZE */
int try_parse_instruction(char* pos, char** next_pos);

#endif	/* INCLUDED_AS_UTIL_H */

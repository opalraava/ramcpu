#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "ramcpu.h"
#include "ramcpu_opcodes.h"
#include "as_util.h"

#define LINE_MAX 1024		/* maximum input line length */

typedef struct {
  char* label;
  char* instruction;
  char* operand;
  char* source;
  char* destination;
} line_parts_t;

void as(FILE* ifp, FILE* ofp, char* ifname);

void
usage(void)
{
  printf("use: as -o [output_file] [input_file.s]\n");
  exit(EXIT_FAILURE);
}

int
main(int argc, char* argv[])
{
  FILE* ifp;
  FILE* ofp;

  /* simple commandline syntax */
  if (argc != 4) usage();
  if (strcmp(argv[1], "-o")) usage();
    

  /* open them files */
  if ((ofp = fopen(argv[2], "wb")) == NULL) {
    perror(argv[2]);
    return EXIT_FAILURE;
  }

  if ((ifp = fopen(argv[3], "r")) == NULL) {
    perror(argv[3]);
    fclose(ofp);
    return EXIT_FAILURE;
  }
    
  /* dive into main processing function */
  as(ifp, ofp, argv[3]);

  fclose(ifp);
  fclose(ofp);
  
  return EXIT_SUCCESS;
}

/* handle syntax errors */
void syntax_error(char* ifname, int line_no, char* msg) {
  printf("%s:%d: %s\n", ifname, line_no, msg);
}
#define fail(msg) do { syntax_error(ifname,line_no,(msg)); exit(EXIT_FAILURE); } while(0)

void as(FILE* ifp, FILE* ofp, char* ifname)
{
  void process_instruction(line_parts_t* line_parts, char* ifname, int line_no);
  
  int line_no = 0;
  char line[LINE_MAX];

  while (fgets(line, sizeof(line), ifp) != NULL)
    {
      char *s;
      line_parts_t line_parts;
      
      ++line_no;

      /* kill empty or comment-only lines */
      trim(line);
      if (line[0] == '\0' || line[0] == ';' || line[0] =='#') continue;

      /* peel the line into parts */
      line_parts.label = line_parts.instruction = line_parts.operand = line_parts.source = line_parts.destination = NULL;

      for (s = line; *s; s++)
	{
	  /* try parse a label */
	  {
	    char* pos = s;
	    char* next_pos;

	    if (try_parse_identifier(pos,&next_pos) == 0) {
	      char* t = next_pos;
		
	      while (*t && isspace(*t)) ++t;
	      if (*t == ':') {
		*t++ = '\0';
		line_parts.label = dupstr(pos);
		
		/* line contains only a label? */
		while (*t && isspace(*t)) ++t;
		if (*t == '\0' || *t == ';' || *t == '#') {
		  process_instruction(&line_parts, ifname, line_no);
		  break;
		}
		else
		  s = t;
	      }
	    }
	  } /* done with label */

	  /* parse instruction */
	  {
	    char* pos = s;
	    char* next_pos;

	    if (try_parse_instruction(pos,&next_pos) == 0) {
	      char* t = next_pos;
	      
	      if (*t && !isspace(*t)) {
		if (*t == ';' || *t == '#') {
		  *t = '\0';
		  line_parts.instruction = dupstr(pos);
		  process_instruction(&line_parts, ifname, line_no);
		  break;
		}
		printf("*nextpos = '%c'\n", *t);
		fail("syntax error following mnemonic");
	      }
	      
	      *t++ = '\0';
	      line_parts.instruction = dupstr(pos);
	      
	      while (*t && isspace(*t)) ++t;
	      
	      if (*t == '\0' || *t == ';' || *t == '#')
		{
		  process_instruction(&line_parts, ifname, line_no);
		  break;
		}
	      else
		s = t;
	    }
	    else
	      fail("not a valid mnemonic");
	  }
	  
	  line_parts.operand = dupstr(s);
	  process_instruction(&line_parts, ifname, line_no);
	  break;
	}
    }
}






void
dump_line_parts(line_parts_t* line_parts, char* ifname, int line_no)
{
  printf("%s:%d: ",ifname, line_no);
  
  if (line_parts->label) printf("label=`%s' ", line_parts->label);
  if (line_parts->instruction) printf("%s=`%s' ", line_parts->instruction[0] == '.'? "directive":"instruction",line_parts->instruction);
  if (line_parts->operand) printf("operand=`%s' ", line_parts->operand);
  if (line_parts->source) printf("source=`%s' ", line_parts->source);
  if (line_parts->destination) printf("destination=`%s'", line_parts->destination);

  printf("\n");
}



void process_instruction(line_parts_t* line_parts, char* ifname, int line_no)
{
  dump_line_parts(line_parts, ifname, line_no);
}
